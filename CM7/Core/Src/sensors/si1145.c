#include <stdio.h>
#include <stdbool.h>

#include "sensors/si1145.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c4;

static bool si1145_initialized = false;

/* Write a byte from a register */
static HAL_StatusTypeDef si1145_write_reg(uint8_t reg, uint8_t val)
{
	return HAL_I2C_Mem_Write(&hi2c4, SI1145_I2C_ADDRESS, reg, 1, &val, 1, 1000);
}

/* Read a byte from a register */
static HAL_StatusTypeDef si1145_read_reg(uint8_t reg, uint8_t *val)
{
	return HAL_I2C_Mem_Read(&hi2c4, SI1145_I2C_ADDRESS, reg, 1, val, 1, 1000);
}

/* Read 16 bytes from a register */
static uint16_t si1145_read16(uint8_t reg)
{
	uint8_t buffer[2];
	if (HAL_I2C_Mem_Read(&hi2c4, SI1145_I2C_ADDRESS, reg, 1, buffer, 2, 1000) == HAL_OK)
	{
		return buffer[0] | (buffer[1] << 8);
	}
	return 0;
}

/* Write a parameter to the si1145 */
static HAL_StatusTypeDef si1145_write_param(uint8_t param, uint8_t value)
{
	// Write the parameter's value to PARAMWR register
    if (si1145_write_reg(SI1145_REG_PARAMWR, value) != HAL_OK)
    {
        return HAL_ERROR;
    }

    // Send the 'parameter set' command
    if (si1145_write_reg(SI1145_REG_COMMAND, param | SI1145_PARAM_SET) != HAL_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/* Reset the si1145 */
static void si1145_reset(void)
{
    // Clear measurement rates and interrupts
    si1145_write_reg(SI1145_REG_MEASRATE0, 0);
    si1145_write_reg(SI1145_REG_MEASRATE1, 0);
    si1145_write_reg(SI1145_REG_IRQEN, 0);
    si1145_write_reg(SI1145_REG_IRQMODE1, 0);
    si1145_write_reg(SI1145_REG_IRQMODE2, 0);
    si1145_write_reg(SI1145_REG_INTCFG, 0);
    si1145_write_reg(SI1145_REG_IRQSTAT, 0xFF);

    // Send reset command
    si1145_write_reg(SI1145_REG_COMMAND, SI1145_RESET);
    HAL_Delay(10);

    // Write hardware key
    si1145_write_reg(SI1145_REG_HWKEY, SI1145_HWKEY_VALUE);
    HAL_Delay(10);
}


BIOME_Error_t SI1145_Init(void)
{
	uint8_t part_id;
	uint8_t channel_list;

    if (si1145_initialized)
    {
        printf("SI1145_Init: Already initialized\r\n");
        return SYS_OK;
    }

    // Test communication
    if (HAL_I2C_IsDeviceReady(&hi2c4, SI1145_I2C_ADDRESS, 3, 1000) != HAL_OK)
    {
        printf("SI1145_Init: Device not ready\r\n");
        return SENS_ERR_INIT;
    }

    // Check part ID

    if (si1145_read_reg(SI1145_REG_PARTID, &part_id) != HAL_OK || part_id != SI1145_EXPECTED_PARTID)
    {
        printf("SI1145_Init: Wrong part ID: 0x%02X (expected 0x%02X)\r\n", part_id, SI1145_EXPECTED_PARTID);
        return SENS_ERR_INIT;
    }

    // Reset the sensor
    si1145_reset();

    // Configure UV coefficients
    si1145_write_reg(SI1145_REG_UCOEFF0, SI1145_UCOEFF0_VALUE);
    si1145_write_reg(SI1145_REG_UCOEFF1, SI1145_UCOEFF1_VALUE);
    si1145_write_reg(SI1145_REG_UCOEFF2, SI1145_UCOEFF2_VALUE);
    si1145_write_reg(SI1145_REG_UCOEFF3, SI1145_UCOEFF3_VALUE);

    // Enable UV, IR, Visible, and Proximity sensors
    channel_list = SI1145_PARAM_CHLIST_ENUV |
                           SI1145_PARAM_CHLIST_ENALSIR |
                           SI1145_PARAM_CHLIST_ENALSVIS |
                           SI1145_PARAM_CHLIST_ENPS1;
    si1145_write_param(SI1145_PARAM_CHLIST, channel_list);

    // Configure interrupts
    si1145_write_reg(SI1145_REG_INTCFG, SI1145_REG_INTCFG_INTOE);
    si1145_write_reg(SI1145_REG_IRQEN, SI1145_REG_IRQEN_ALSEVERYSAMPLE);

    // Set measurement rate (255 * 31.25µs = 8ms)
    si1145_write_reg(SI1145_REG_MEASRATE0, 0xFF);

    // Start auto measurement mode
    si1145_write_reg(SI1145_REG_COMMAND, SI1145_PSALS_AUTO);

    printf("Successfully initialized SI1145.\r\n");
    si1145_initialized = true;
    return SYS_OK;
}

BIOME_Error_t SI1145_ReadData(SI1145_Data_t *data)
{
    if (!si1145_initialized) { return SENS_ERR_NOT_RDY; }
    if (!data) { return SYS_ERR_NULL_PTR; }

    // Read sensor data
    data->uv_index = si1145_read16(SI1145_REG_UVINDEX0);
    data->ir_light = si1145_read16(SI1145_REG_ALSIRDATA0);
    data->visible_light = si1145_read16(SI1145_REG_ALSVISDATA0);
    data->proximity_1 = si1145_read16(SI1145_REG_PS1DATA0);
    data->proximity_2 = si1145_read16(SI1145_REG_PS2DATA0);
    data->proximity_3 = si1145_read16(SI1145_REG_PS3DATA0);

    data->data_valid = 1;

    return SYS_OK;
}

BIOME_Error_t SI1145_TestCommunication(void)
{
    printf("Testing SI1145 I2C communication...\r\n");

    HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(&hi2c4, SI1145_I2C_ADDRESS, 3, 1000);

    if (status != HAL_OK)
    {
        printf("SI1145 communication test FAILED (status: %d).\r\n", status);
        return SENS_ERR_NOT_RDY;
    }

    printf("SI1145 communication test PASSED.\r\n");
    return SYS_OK;
}
