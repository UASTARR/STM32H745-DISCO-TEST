#ifndef MPL3115A2_H
#define MPL3115A2_H

#include "error.h"
#include "main.h"
#include "stdint.h"
#include "stdbool.h"

/* I2C Address for the MPL3115A2 */
#define MPL3115A2_I2C_ADDRESS        (0x60 << 1)

/* MPL3115A2 Register Addresses */
#define MPL3115A2_REGISTER_STATUS        0x00
#define MPL3115A2_REGISTER_PRESSURE_MSB  0x01
#define MPL3115A2_REGISTER_PRESSURE_CSB  0x02
#define MPL3115A2_REGISTER_PRESSURE_LSB  0x03
#define MPL3115A2_REGISTER_TEMP_MSB      0x04
#define MPL3115A2_REGISTER_TEMP_LSB      0x05
#define MPL3115A2_REGISTER_DR_STATUS     0x06
#define MPL3115A2_WHOAMI                 0x0C
#define MPL3115A2_PT_DATA_CFG            0x13
#define MPL3115A2_CTRL_REG1              0x26
#define MPL3115A2_CTRL_REG2              0x27
#define MPL3115A2_CTRL_REG3              0x28
#define MPL3115A2_CTRL_REG4              0x29
#define MPL3115A2_CTRL_REG5              0x2A

/* Status Register Bits */
#define MPL3115A2_REGISTER_STATUS_TDR    0x02
#define MPL3115A2_REGISTER_STATUS_PDR    0x04
#define MPL3115A2_REGISTER_STATUS_PTDR   0x08

/* PT Data Configuration Bits */
#define MPL3115A2_PT_DATA_CFG_TDEFE      0x01
#define MPL3115A2_PT_DATA_CFG_PDEFE      0x02
#define MPL3115A2_PT_DATA_CFG_DREM       0x04

/* Control Register 1 Bits */
#define MPL3115A2_CTRL_REG1_SBYB         0x01
#define MPL3115A2_CTRL_REG1_OST          0x02
#define MPL3115A2_CTRL_REG1_RST          0x04
#define MPL3115A2_CTRL_REG1_RAW          0x40
#define MPL3115A2_CTRL_REG1_ALT          0x80
#define MPL3115A2_CTRL_REG1_BAR          0x00

/* Oversampling Values */
#define MPL3115A2_CTRL_REG1_OS1          0x00
#define MPL3115A2_CTRL_REG1_OS2          0x08
#define MPL3115A2_CTRL_REG1_OS4          0x10
#define MPL3115A2_CTRL_REG1_OS8          0x18
#define MPL3115A2_CTRL_REG1_OS16         0x20
#define MPL3115A2_CTRL_REG1_OS32         0x28
#define MPL3115A2_CTRL_REG1_OS64         0x30
#define MPL3115A2_CTRL_REG1_OS128        0x38

/* Barometric Pressure Settings */
#define MPL3115A2_BAR_IN_MSB             0x14
#define MPL3115A2_BAR_IN_LSB             0x15


/* WHOAMI Expected Value */
#define MPL3115A2_WHOAMI_VALUE            0xC4



typedef struct {
	float pressure;
	float temperature;
	float altitude;
	uint8_t data_valid;
} MPL3115A2_Data_t;


BIOME_Error_t MPL3115A2_Init(void);
BIOME_Error_t MPL3115A2_ReadData(MPL3115A2_Data_t *data);
BIOME_Error_t MPL3115A2_TestCommunication(void);

#endif // MPL3115A2_H
