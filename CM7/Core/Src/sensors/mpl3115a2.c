#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "sensors/mpl3115a2.h"

static bool mpl3115a2_initialized = false;

static HAL_StatusTypeDef mpl3115a2_write_reg(uint8_t reg, uint8_t val)
{
	return HAL_I2C_Mem_Write(&hi2c4, MPL3115A2_I2C_ADDRESS, reg, 1, &val, 1, 1000);
}

static HAL_StatusTypeDef mpl3115a2_write_multiple_reg(uint8_t reg, uint8_t *val, uint8_t len)
{
	return HAL_I2C_Mem_Write(&hi2c4, MPL3115A2_I2C_ADDRESS, reg, 1, val, len, 1000);
}

static HAL_StatusTypeDef mpl3115a2_read_reg(uint8_t reg, uint8_t *val)
{
	return HAL_I2C_Mem_Read(&hi2c4, MPL3115A2_I2C_ADDRESS, reg, 1, val, 1, 1000);
}

/* length is the number of bytes to read*/
static HAL_StatusTypeDef mpl3115a2_read_multiple_reg(uint8_t reg, uint8_t *val, uint8_t len)
{
	return HAL_I2C_Mem_Read(&hi2c4, MPL3115A2_I2C_ADDRESS, reg, 1, val, len, 1000);
}

static HAL_StatusTypeDef mpl3115a2_is_conversion_complete(void)
{
	uint8_t status = 0;
	if (mpl3115a2_read_reg(MPL3115A2_REGISTER_STATUS, &status) != HAL_OK)
	{
		return false;
	}

	return ((status & MPL3115A2_REGISTER_STATUS_PTDR) != 0);
}

static void mpl3115a2_initialize_read(void)
{
	uint8_t ctrl_reg1 = 0;

	do {
		if (mpl3115a2_read_reg(MPL3115A2_CTRL_REG1, &ctrl_reg1) != HAL_OK)
		{
			return;
		}
	} while (ctrl_reg1 & MPL3115A2_CTRL_REG1_OST);

	ctrl_reg1 |= MPL3115A2_CTRL_REG1_OST;
	if (mpl3115a2_write_reg(MPL3115A2_CTRL_REG1, ctrl_reg1))
	{
		printf("MPL3115A2: Error initializing read on MPL3115A2\r\n");
	}
	return;
}

BIOME_Error_t MPL3115A2_Init(void)
{
	uint8_t whoami = 0;
	uint8_t ctrl_reg1 = 0;
	uint8_t pt_data_cfg;
	uint8_t set_press_buf[3];
	
	if (mpl3115a2_initialized) { return SYS_OK; }
	
	if (HAL_I2C_IsDeviceReady(&hi2c4, MPL3115A2_I2C_ADDRESS, 3, 1000) != HAL_OK)
	{
		printf("MPL3115A2: INITIALIZATION FAILED.\r\n");
		return SENS_ERR_INIT;
	}
	
	if (mpl3115a2_read_reg(MPL3115A2_WHOAMI, &whoami) != HAL_OK)
	{
		printf("MPL3115A2: READ WHOAMI FAILED.\r\n");
		return SENS_ERR_INIT;
	}

	if (whoami != MPL3115A2_WHOAMI_VALUE)
	{
		printf("MPL3115A2: WRONG WHOAMI: 0x%02X (expected 0x%02X).\r\n", whoami, MPL3115A2_WHOAMI_VALUE);
		return SENS_ERR_INIT;
	}

//	if (mpl3115a2_write_reg(MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_RST) != HAL_OK)
//	{
//		printf("MPL3115A2: ERROR RESETTING MPL3115A2.\r\n");
//		return SENS_ERR_RST;
//	}

	do {
		HAL_Delay(10);
		if (mpl3115a2_read_reg(MPL3115A2_CTRL_REG1, &ctrl_reg1) != HAL_OK)
		{
			printf("MPL3115A2: FAILED TO READ CTRL_REG1 WHILE RESETTING SENSOR.\r\n");
			return SENS_ERR_RST;
		}
	} while (ctrl_reg1 & MPL3115A2_CTRL_REG1_RST);
	

	ctrl_reg1 = MPL3115A2_CTRL_REG1_OS128 | MPL3115A2_CTRL_REG1_ALT;
	if (mpl3115a2_write_reg(MPL3115A2_CTRL_REG1, ctrl_reg1) != HAL_OK)
	{
		printf("MPL3115A2: ERROR SETTING OVERSAMPLING AND ALTITUDE MODE.\r\n");
		return SENS_ERR_INIT;
	}

	pt_data_cfg = MPL3115A2_PT_DATA_CFG_TDEFE | MPL3115A2_PT_DATA_CFG_PDEFE |
				  MPL3115A2_PT_DATA_CFG_DREM;

	if (mpl3115a2_write_reg(MPL3115A2_PT_DATA_CFG, pt_data_cfg) != HAL_OK)
	{
		printf("MPL3115A2: ERROR SETTING CONFIGURATION.\r\n");
		return SENS_ERR_CONF;
	}

//	set_press_buf[0] = MPL3115A2_BAR_IN_MSB;
//	set_press_buf[1] = EDM_SEA_LEVEL_PRESSURE >> 8;
//	set_press_buf[2] = EDM_SEA_LEVEL_PRESSURE & 0xFF;
//
//	if (mpl3115a2_write_multiple_reg(MPL3115A2_BAR_IN_MSB, set_press_buf, 3) != HAL_OK)
//	{
//		printf("MPL3115A2: ERROR SETTING SEA LEVELPRESSURE.\r\n");
//	}

	printf("MPL3115A2: SUCCESSFULLY INITIALIZED.\r\n");
	mpl3115a2_initialized = true;
	return SYS_OK;
}


BIOME_Error_t MPL3115A2_ReadData(MPL3115A2_Data_t *data)
{
	uint16_t timeout = 1000;
	
	uint8_t buf[5] = {MPL3115A2_REGISTER_PRESSURE_MSB, 0, 0, 0, 0};

	uint32_t pressure_raw;
	uint32_t temp_raw;
	uint32_t altitude_raw;

	if (!mpl3115a2_initialized) { return SENS_ERR_NOT_RDY; }
	if (!data) { return SYS_ERR_NULL_PTR; }

	data->temperature = 0;
	data->pressure = 0;
	data->altitude = 0;

	mpl3115a2_initialize_read();

	while (!mpl3115a2_is_conversion_complete() && timeout > 0)
	{
		HAL_Delay(10);
		timeout -= 10;
	}

	if (timeout == 0)
	{
		printf("MPL3115A2: CONVERSION TIMED OUT.\r\n");
		return SENS_ERR_TIMEOUT;
	}

	if (mpl3115a2_read_multiple_reg(MPL3115A2_REGISTER_PRESSURE_MSB, buf, 5) != HAL_OK)
	{
		printf("MPL3115A2: ERROR READING DATA.\r\n");
		return SENS_ERR_READ;
	}

	pressure_raw = ((uint32_t)buf[0] << 16) | ((uint32_t)buf[1] << 8) |
			(uint32_t)buf[2];
	temp_raw = ((uint32_t)buf[3] << 8) | (uint32_t)buf[4];
	altitude_raw = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
			((uint32_t)buf[2] << 8);

	data->pressure = (float)pressure_raw / 6400.0f;
	data->temperature = (float)temp_raw / 256.0f;
	data->altitude = (float)altitude_raw / 65536.0f;

	data->data_valid = 1;

	return SYS_OK;
}


BIOME_Error_t MPL3115A2_TestCommunication(void)
{
	HAL_StatusTypeDef status;
	printf("Testing MPL3115A2 I2C communication...\r\n");

	status = HAL_I2C_IsDeviceReady(&hi2c4, MPL3115A2_I2C_ADDRESS, 3, 1000);

	if (status != HAL_OK)
	{
		printf("MPL3115A2 communication test FAILED (STATUS: %d)\r\n", status);
		return SENS_ERR_NOT_RDY;
	}

	// Also test WHOAMI register
	uint8_t whoami;
	if (mpl3115a2_read_reg(MPL3115A2_WHOAMI, &whoami) != HAL_OK)
	{
		printf("MPL3115A2 communication test FAILED (STATUS: %d)\r\n", status);
		return SENS_ERR_NOT_RDY;
	}

	if (whoami != MPL3115A2_WHOAMI_VALUE)
	{
		printf("MPL3115A2 communication test FAILED (WHOAMI: 0x%02X)\r\n", whoami);
		return SENS_ERR_NOT_RDY;
	}

	printf("MPL3115A2 communication test PASSED!\r\n");
	return SYS_OK;
}

