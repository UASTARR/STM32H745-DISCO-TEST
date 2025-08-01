/*
 * @file sensor_init.c
 * @author Miro Straszynski
 * @brief Initialization of sensors for BIOME-III
 * @version 0.1
 * @date 2025-07-19
 * 
 * @copyright ---
 * 
 */

#ifndef ERROR_H
#include "error.h"
#endif // ERROR_H

#include "main.h"
#include "sensors/bme680.h"
#include "sensors/si1145.h"
#include "sensors/sgp30.h"
#include "sensors/mmc56x3.h"
#include "sensors/mpl3115a2.h"

/*
 * @brief Initializes the sensors used by BIOME-III
 *
 * @return A BIOME error code
 *
 */
BIOME_Error_t sensor_init(void) 
{
	int result;

#ifdef USE_BME680
    result = BME680_Init();
    if (result != SYS_OK) { return result; }
#endif // USE_BME680

#ifdef USE_SI1145
    result = SI1145_Init();
    if (result != SYS_OK) { return result; }
#endif // USE_SI1145

#ifdef USE_SGP30
    result = SGP30_Init();
    if (result != SYS_OK) { return result; }
#endif // USE_SGP30

#ifdef USE_MMC56X3
    result = MMC56X3_Init();
    if (result != SYS_OK) { return result; }
#endif // USE_MMC56X3

#ifdef USE_MPL3115A2
    result = MPL3115A2_Init();
    if (result != SYS_OK) { return result; }
#endif // USE_MPL3115A2

    return SYS_OK;
}
