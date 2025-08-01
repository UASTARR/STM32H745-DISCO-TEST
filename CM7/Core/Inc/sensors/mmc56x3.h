#ifndef MMC56X3_H
#define MMC56X3_H

#include "main.h"
#include "error.h"
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    int32_t x_magnetic;        // X-axis magnetic field (raw counts)
    int32_t y_magnetic;        // Y-axis magnetic field (raw counts)
    int32_t z_magnetic;        // Z-axis magnetic field (raw counts)
    uint8_t temperature;       // Temperature reading (8-bit)
    uint8_t data_valid;        // 1 if data is valid, 0 if invalid/stale
} MMC56X3_Data_t;


/* MMC56X3 I2C Address */
#define MMC56X3_ADDR                     0x30
#define MMC56X3_I2C_ADDRESS             (MMC56X3_ADDR << 1)

/* MMC56X3 Registers */
#define MMC56X3_OUT_X_L                 0x00
#define MMC56X3_OUT_X_H                 0x01
#define MMC56X3_OUT_Y_L                 0x02
#define MMC56X3_OUT_Y_H                 0x03
#define MMC56X3_OUT_Z_L                 0x04
#define MMC56X3_OUT_Z_H                 0x05
#define MMC56X3_OUT_TEMP                0x09
#define MMC56X3_STATUS_REG              0x18
#define MMC56X3_CTRL0_REG               0x1B
#define MMC56X3_CTRL1_REG               0x1C
#define MMC56X3_CTRL2_REG               0x1D
#define MMC56X3_PRODUCT_ID              0x39

/* MMC56X3 Commands */
#define MMC56X3_CMD_SET                 0x08
#define MMC56X3_CMD_RESET               0x10
#define MMC56X3_CMD_MEASURE             0x01

/* MMC56X3 Constants */
#define MMC56X3_CHIP_ID	                0x10
#define MMC56X3_MAGNETIC_SCALE          0.00625f  // LSB to uT conversion
#define MMC56X3_CENTER_OFFSET			(1 << 19)



BIOME_Error_t MMC56X3_Init(void);
BIOME_Error_t MMC56X3_ReadData(MMC56X3_Data_t *data);
BIOME_Error_t MMC56X3_TestCommunication(void);

static inline float MMC56X3_GetMagnetic_uT(int32_t raw_value) 
{
    return (float)raw_value * MMC56X3_MAGNETIC_SCALE;
}

static inline float MMC56X3_GetMagnitude(float x, float y, float z) 
{
    return sqrtf(x*x + y*y + z*z);
}

static inline float MMC56X3_GetHeading(float x, float y) 
{
    float heading = atan2f(y, x) * 180.0f / 3.14159f;
    if (heading < 0) heading += 360.0f;
    return heading;
}

#ifdef __cplusplus
}
#endif

#endif // MMC56X3_H

