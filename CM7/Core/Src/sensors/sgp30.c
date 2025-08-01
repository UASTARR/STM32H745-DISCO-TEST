#include <stdio.h>

#include "sensors/sgp30.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c4;

static bool sgp30_initialized = false;

static HAL_StatusTypeDef sgp30_write_cmd(uint16_t cmd)
{
	/* Build up a two byte buffer for this 16 bit command */
	uint8_t cmd_buf[2] = {(cmd >> 8) & 0xFF, cmd & 0xFF};
	return HAL_I2C_Master_Transmit(&hi2c4, SGP30_I2C_ADDRESS, cmd_buf, 2, 1000);
}

static HAL_StatusTypeDef sgp30_read_data(uint8_t *data, uint16_t len)
{
	return HAL_I2C_Master_Receive(&hi2c4, SGP30_I2C_ADDRESS, data, len, 1000);
}

/*
 * The SGP30 sends data with a CRC8 checksum, so we might as well handle that
 * and attempt to filter out bad data caused by communication issues.
 */
static uint8_t sgp30_crc8(const uint8_t *data, uint8_t len)
{
	uint8_t crc = 0xFF;

	for (uint8_t idx = 0; idx < len; idx++)
	{
		crc ^= data[idx];
		for (uint8_t bit = 8; bit > 0; bit--)
		{
			if (crc & 0x80)
			{
			crc = (crc << 1) ^ 0x31;
			} else {
				crc = (crc << 1);
			}
		}
	}
	return crc;
}

static bool sgp30_check_crc(const uint8_t *data)
{
	uint8_t calculated_crc = sgp30_crc8(data, 2);
	uint8_t received_crc = data[2];

	if (calculated_crc != received_crc)
	{
		printf("SGP30: CRC Error: Expected 0x%02X, got 0x%02X for data [0x%2X, 0x%2X]\r\n",
				calculated_crc, received_crc, data[0], data[1]);
		return false;
	}
	return true;
}


BIOME_Error_t SGP30_Init(void)
{
	if (sgp30_initialized)
	{
		printf("SGP30: Sensor already initialized.");
		return SYS_OK;
	}

	if (HAL_I2C_IsDeviceReady(&hi2c4, SGP30_I2C_ADDRESS, 3, 1000) != HAL_OK)
	{
		printf("SGP30: DEVICE IS NOT READY.\r\n");
		return SENS_ERR_INIT;
	}

	if (sgp30_write_cmd(SGP30_CMD_IAQ_INIT) != HAL_OK)
	{
		printf("SGP30: IAQ INITIALIZATION FAILED.\r\n");
		return SENS_ERR_INIT;
	}

	/* Wait 10ms for initialization */
	HAL_Delay(10);

	printf("SGP30: SUCCESSFULLY INITIALIZED.\r\n");
	sgp30_initialized = true;
	return SYS_OK;
}


BIOME_Error_t SGP30_ReadData(SGP30_Data_t *data)
{
    if (!sgp30_initialized) { return SENS_ERR_NOT_RDY; }
    if (!data) { return SYS_ERR_NULL_PTR; }

    uint8_t read_buf[6];

    // Send measure command
    if (sgp30_write_cmd(SGP30_CMD_IAQ_MEASURE) != HAL_OK)
    {
        printf("SGP30_ReadData: MEASURE COMMAND FAILED.\r\n");
        return SENS_ERR_NOT_RDY;
    }

    // Wait for measurement (12ms)
    HAL_Delay(15);

    // Read 6 bytes for CO2 (high), CO2 (low), CRC, TVOC (high), TVOC (low), CRC
    if (sgp30_read_data(read_buf, 6) != HAL_OK)
    {
        printf("SGP30_ReadData: Read failed\r\n");
        return SENS_ERR_NOT_RDY;
    }

    // Verify the CRC for CO2 data (bytes 0-2, with byte 2 being the CRC checksum)
    if (!sgp30_check_crc(&read_buf[0]))
    {
        printf("SGP30_ReadData: CO2 CRC verification failed\r\n");
        return SENS_ERR_NOT_RDY;
    }

    // Verify CRC for TVOC data (bytes 3-5, with byte 5 being the CRC checksum)
    if (!sgp30_check_crc(&read_buf[3]))
    {
        printf("SGP30_ReadData: TVOC CRC verification failed\r\n");
        return SENS_ERR_NOT_RDY;
    }


    data->co2_equiv_ppm = (read_buf[0] << 8) | read_buf[1];
    data->tvoc_ppb = (read_buf[3] << 8) | read_buf[4];
    data->data_valid = 1;

    return SYS_OK;
}


BIOME_Error_t SGP30_TestCommunication(void) {
	HAL_StatusTypeDef status;
    printf("Testing SGP30 I2C communication...\r\n");

    status = HAL_I2C_IsDeviceReady(&hi2c4, SGP30_I2C_ADDRESS, 3, 1000);

    if (status != HAL_OK)
    {
        printf("SGP30 communication test FAILED (STATUS: %d)\r\n", status);
        return SENS_ERR_NOT_RDY;
    }

    printf("SGP30 communication test PASSED!\r\n");
    return SYS_OK;
}











