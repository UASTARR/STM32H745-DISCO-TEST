#ifndef BME680_H
#define BME680_H

#include "error.h"

#include "../../../Drivers/BME68X/Inc/bme68x.h"

/*
 * Define the i2c address for the BME680.
 * This needs to be shifted left by one bit, because I2C addresses
 * are 7-bits by default, but the HAL expects 8-bits, with the LSB
 * being the R/W bit. This is set by HAL, however we need to 'create'
 * the space for the 8th bit.
 */
#define BME680_I2C_ADDRESS (BME68X_I2C_ADDR_HIGH << 1)

/*
 * Struct to hold data retrieved from
 * the BME680.
 */
typedef struct {
    float temperature;    // °C
    float pressure;      // Pa
    float humidity;      // %RH
    float gas_resistance; // Ohms
    uint8_t data_valid;
} BME680_Data_t;


/* Public functions */
BIOME_Error_t BME680_Init(void);
BIOME_Error_t BME680_ReadData(BME680_Data_t *data);
BIOME_Error_t BME680_TestCommunication(void);

#endif // BME680_H
