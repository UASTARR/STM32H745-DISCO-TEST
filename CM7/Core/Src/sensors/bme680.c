#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "sensors/bme680.h"
#include "main.h"
#include "error.h"
#include "../../Drivers/BME68X/Inc/bme68x.h"
#include "../../Drivers/BME68X/Inc/bme68x_defs.h"

static struct bme68x_dev bme680_dev;
static bool bme680_initialized = false;

static int8_t bme680_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
static int8_t bme680_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
static void bme680_delay_us(uint32_t period, void *intf_ptr);

/*
 * BME680 I2C read function.
 * This is used internally by the BME680 library to read data from the sensor.
 */
static int8_t bme680_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef*)intf_ptr;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, BME68X_I2C_ADDR_HIGH << 1,
                                               reg_addr, 1, reg_data, len, 100);

    return (status == HAL_OK) ? BME68X_INTF_RET_SUCCESS : -1;
}

/*
 * BME680 I2C write function.
 * This is used internally by the BME680 library to write to the sensor.
 */
static int8_t bme680_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef*)intf_ptr;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hi2c, BME68X_I2C_ADDR_HIGH << 1,
                                                reg_addr, 1, (uint8_t*)reg_data, len, 100);

    return (status == HAL_OK) ? BME68X_INTF_RET_SUCCESS : -1;
}

/* Microsecond delay function needed by the BME680 library. */
static void bme680_delay_us(uint32_t period, void *intf_ptr)
{
    uint32_t delay_ms = (period / 1000) + ((period % 1000) ? 1 : 0);
    if (delay_ms == 0) delay_ms = 1;

    HAL_Delay(delay_ms);
}


/* Public function to initialize the BME680 */
BIOME_Error_t BME680_Init(void)
{
	int8_t result;
	HAL_StatusTypeDef i2c_status;
	struct bme68x_conf conf;
	struct bme68x_heatr_conf heatr_conf;

	if (bme680_initialized) { return SYS_OK; }

	bme680_dev.intf = BME68X_I2C_INTF;
	bme680_dev.intf_ptr = &hi2c4;
	bme680_dev.read = bme680_i2c_read;
	bme680_dev.write = bme680_i2c_write;
	bme680_dev.delay_us = bme680_delay_us;
	bme680_dev.amb_temp = 25;

	result = bme68x_init(&bme680_dev);
	if (result != BME68X_OK)
	{
		printf("INITIALIZATION FAILED WITH ERROR CODE %d\r\n", result);
		return SENS_ERR_INIT;
	}
	else
	{
		i2c_status = HAL_I2C_IsDeviceReady(&hi2c4, BME680_I2C_ADDRESS, 3, 1000);
		printf("HAL_I2C_IsDeviceReady Result for BME680: %d\r\n", i2c_status);
	}

	conf.filter = BME68X_FILTER_OFF;
	conf.odr = BME68X_ODR_NONE;
	conf.os_hum = BME68X_OS_2X;
	conf.os_pres = BME68X_OS_4X;
	conf.os_temp = BME68X_OS_8X;

	result = bme68x_set_conf(&conf, &bme680_dev);
	if (result != BME68X_OK)
	{
		printf("BME680: CONFIG FAILED: %d\r\n", result);
		return SENS_ERR_CONF;
	}

	heatr_conf.enable = BME68X_ENABLE;
	heatr_conf.heatr_temp = 300;
	heatr_conf.heatr_dur = 100;

	result = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme680_dev);
	if (result != BME68X_OK)
	{
		printf("BME680: HEATER CONFIG FAILED: %d\r\n", result);
		return SENS_ERR_CONF;
	}

	printf("BME680 INITIALIZATION COMPLETE!\r\n");
	bme680_initialized = true;
	return SYS_OK;
}

BIOME_Error_t BME680_ReadData(BME680_Data_t *data)
{
	int8_t result;
	uint8_t num_fields;
	struct bme68x_data retrieved_data;

	if (!bme680_initialized) { return SENS_ERR_NOT_RDY; }
	if (!data) { return SYS_ERR_NULL_PTR; }

	result = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme680_dev);
	if (result == BME68X_E_NULL_PTR) { return SYS_ERR_NULL_PTR; }
	else if (result != BME68X_OK) { return SENS_ERR_NOT_RDY; }

	HAL_Delay(100);
	result = bme68x_get_data(BME68X_FORCED_MODE, &retrieved_data, &num_fields, &bme680_dev);
	if (result == BME68X_E_NULL_PTR) { return SYS_ERR_NULL_PTR; }
	else if (result != BME68X_OK) { return SENS_ERR_NOT_RDY; }

	data->temperature = retrieved_data.temperature;
	data->pressure = retrieved_data.pressure;
	data->humidity = retrieved_data.humidity;
	data->gas_resistance = retrieved_data.gas_resistance;

	/*
	 * If we've retrieved new data, the new data mask will be set. If it
	 * isn't set, then no new data has been retrieved. The caller of
	 * BME680_ReadData should be clearing data_valid before reading.
	 */
	data->data_valid = (retrieved_data.status & BME68X_NEW_DATA_MSK) ? 1 : 0;

	return SYS_OK;
}

BIOME_Error_t BME680_TestCommunication(void)
{
	HAL_StatusTypeDef status;
	printf("Testing SI1145 I2C communication...\r\n");

	status = HAL_I2C_IsDeviceReady(&hi2c4, BME68X_I2C_ADDR_HIGH, 3, 100);

	if (status != HAL_OK)
	{

		printf("BME680 communication test FAILED (status: %d)\r\n", status);
		return SENS_ERR_NOT_RDY; }

	printf("BME680 communication test PASSED.");
	return SYS_OK;
}





