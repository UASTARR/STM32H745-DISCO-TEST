#include <stdio.h>
#include <math.h>

#include "sensors/mmc56x3.h"
#include "main.h"


extern I2C_HandleTypeDef hi2c4;

static bool mmc56x3_initialized = false;

static HAL_StatusTypeDef mmc56x3_write_reg(uint8_t reg, uint8_t val) 
{
    return HAL_I2C_Mem_Write(&hi2c4, MMC56X3_I2C_ADDRESS, reg, 1, &val, 1, 1000);
}

static HAL_StatusTypeDef mmc56x3_read_reg(uint8_t reg, uint8_t *val) 
{
    return HAL_I2C_Mem_Read(&hi2c4, MMC56X3_I2C_ADDRESS, reg, 1, val, 1, 1000);
}

static HAL_StatusTypeDef mmc56x3_read_multiple(uint8_t reg, uint8_t *data, uint16_t len) 
{
    return HAL_I2C_Mem_Read(&hi2c4, MMC56X3_I2C_ADDRESS, reg, 1, data, len, 1000);
}

static void mmc56x3_convert_20bit(uint8_t *buffer, int32_t *x, int32_t *y, int32_t *z) {
    // Adafruit format: 20-bit data with extra bits in bytes 6,7,8
    *x = (uint32_t)buffer[0] << 12 | (uint32_t)buffer[1] << 4 | (uint32_t)buffer[6] >> 4;
    *y = (uint32_t)buffer[2] << 12 | (uint32_t)buffer[3] << 4 | (uint32_t)buffer[7] >> 4;
    *z = (uint32_t)buffer[4] << 12 | (uint32_t)buffer[5] << 4 | (uint32_t)buffer[8] >> 4;
    
    // Fix center offsets (subtract 2^19)
    *x -= MMC56X3_CENTER_OFFSET;
    *y -= MMC56X3_CENTER_OFFSET;
    *z -= MMC56X3_CENTER_OFFSET;
}

BIOME_Error_t MMC56X3_Init(void) 
{
    uint8_t product_id;

    if (mmc56x3_initialized) 
    {
        printf("MMC56X3: Sensor already initialized.\r\n");
        return SYS_OK;
    }

    if (HAL_I2C_IsDeviceReady(&hi2c4, MMC56X3_I2C_ADDRESS, 3, 1000) != HAL_OK) 
    {
        printf("MMC56X3: DEVICE IS NOT READY.\r\n");
        return SENS_ERR_INIT;
    }

    /* Reset the sensor */
	mmc56x3_write_reg(MMC56X3_CTRL1_REG, 0x80);


    /* Set measurement rate (100Hz) */
    if (mmc56x3_write_reg(MMC56X3_CTRL1_REG, 0x80) != HAL_OK)
    {
        printf("MMC56X3: FAILED TO CONFIGURE CTRL1.\r\n");
        return SENS_ERR_INIT;
    }

    mmc56x3_write_reg(MMC56X3_CTRL0_REG, 0x08);  // Set
	HAL_Delay(1);
	mmc56x3_write_reg(MMC56X3_CTRL0_REG, 0x10);  // Reset
	HAL_Delay(1);

    mmc56x3_write_reg(MMC56X3_CTRL0_REG, 0x00);  // Clear continuous mode
    mmc56x3_write_reg(MMC56X3_CTRL2_REG, 0x00);  // Clear CMM enable

//    /* Check product ID */
//	if (mmc56x3_read_reg(MMC56X3_PRODUCT_ID, &product_id) != HAL_OK)
//	{
//		printf("MMC56X3: FAILED TO READ PRODUCT ID.\r\n");
//		return SENS_ERR_INIT;
//	}
//
//	if (product_id != MMC56X3_CHIP_ID)
//	{
//		printf("MMC56X3: WRONG PRODUCT ID: 0x%02X (expected 0x%02X)\r\n",
//			   product_id, MMC56X3_CHIP_ID);
//		return SENS_ERR_INIT;
//	}

    printf("MMC56X3: INITIALIZED SUCCESSFULLY.\r\n");
    mmc56x3_initialized = true;
    return SYS_OK;
}

BIOME_Error_t MMC56X3_ReadData(MMC56X3_Data_t *data) {
    if (!mmc56x3_initialized) { return SENS_ERR_NOT_RDY; }
    if (!data) { return SYS_ERR_NULL_PTR; }
    
    uint8_t read_buf[9];
    uint8_t status;

    mmc56x3_write_reg(MMC56X3_CTRL0_REG, MMC56X3_CMD_MEASURE);

    do
    {
    	HAL_Delay(5);
    	mmc56x3_read_reg(MMC56X3_STATUS_REG, &status);
    } while (!(status & 0x40)); /* bit gets set when done */


    /* Read 9 bytes (3 bytes per axis) */
    if (mmc56x3_read_multiple(MMC56X3_OUT_X_L, read_buf, 9) != HAL_OK)
    {
        printf("MMC56X3_ReadData: READ COMMAND FAILED.\r\n");
        return SENS_ERR_NOT_RDY;
    }

    mmc56x3_convert_20bit(read_buf, &data->x_magnetic, &data->y_magnetic, &data->z_magnetic);
    data->data_valid = 1;

    /* Read temperature */
    mmc56x3_read_reg(MMC56X3_OUT_TEMP, &data->temperature);

    return SYS_OK;
}

BIOME_Error_t MMC56X3_TestCommunication(void) {
    HAL_StatusTypeDef status;
    uint8_t product_id;

    printf("Testing MMC56X3 I2C communication...\r\n");
    
    status = HAL_I2C_IsDeviceReady(&hi2c4, MMC56X3_I2C_ADDRESS, 3, 1000);
    
    if (status != HAL_OK) {
        printf("MMC56X3 communication test FAILED (status: %d)\r\n", status);
        return SENS_ERR_NOT_RDY;
    }

    /* Try to read the product ID */
    if (mmc56x3_read_reg(MMC56X3_PRODUCT_ID, &product_id) == HAL_OK) {
        printf("MMC56X3 communication test PASSED! PRODUCT ID: 0x%02X\r\n", product_id);
    } else {
        printf("MMC56X3 communication test FAILED - CANNOT READ PRODUCT ID.\r\n");
        return SENS_ERR_NOT_RDY;
    }

    return SYS_OK;
}






