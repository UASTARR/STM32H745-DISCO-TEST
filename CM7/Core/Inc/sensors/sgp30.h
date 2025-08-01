#ifndef SGP30_H
#define SGP30_H

#include "main.h"
#include "error.h"
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint16_t co2_equiv_ppm;
	uint16_t tvoc_ppb;
	uint16_t data_valid;
} SGP30_Data_t;

/* I2C Address for the SGP30 */
#define SGP30_I2C_ADDRESS			(0x58 << 1)

/* Commands for the SGP30 */
#define SGP30_CMD_IAQ_INIT          0x2003
#define SGP30_CMD_IAQ_MEASURE       0x2008
#define SGP30_CMD_GET_SERIAL_ID     0x3682
#define SGP30_CMD_MEASURE_TEST      0x2032
#define SGP30_CMD_GET_FEATURESET    0x202F

/* Expected values from the sgp30 */
#define SGP30_SELFTEST_OK			0xD400


BIOME_Error_t SGP30_Init(void);
BIOME_Error_t SGP30_ReadData(SGP30_Data_t *data);
BIOME_Error_t SGP30_TestCommunication(void);

static inline float SGP_GetCO2_ppm(uint16_t co2_raw) { return (float)co2_raw; }

static inline float SGP_GetTVOC_ppb(uint16_t tvoc_raw) { return (float)tvoc_raw; }

#ifdef __cplusplus
}
#endif

#endif // SGP30_H
