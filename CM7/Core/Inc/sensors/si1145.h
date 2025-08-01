#ifndef SI1145_H
#define SI1145_H

#include "error.h"
#include "main.h"
/*
 * I2C address for the SI1145 (specifically for addressing
 * on STM32.
 */
#define SI1145_I2C_ADDRESS (0x60 << 1)

/*
 * The following section is for the most part taken
 * directly from the Adafruit SI1145 C++ library, as
 * they reference command definitions or memory locations
 * on the SI1145.
 */

/* SI1145 Commands */
#define SI1145_PARAM_QUERY				0x80
#define SI1145_PARAM_SET				0xA0
#define SI1145_NOP                      0x00
#define SI1145_RESET                    0x01
#define SI1145_BUSADDR                  0x02
#define SI1145_PS_FORCE                 0x05
#define SI1145_ALS_FORCE                0x06
#define SI1145_PSALS_FORCE              0x07
#define SI1145_PS_PAUSE                 0x09
#define SI1145_ALS_PAUSE                0x0A
#define SI1145_PSALS_PAUSE              0x0B
#define SI1145_PS_AUTO                  0x0D
#define SI1145_ALS_AUTO                 0x0E
#define SI1145_PSALS_AUTO               0x0F
#define SI1145_GET_CAL                  0x12

/* SI1145 Parameters */
#define SI1145_PARAM_I2CADDR            0x00
#define SI1145_PARAM_CHLIST             0x01
#define SI1145_PARAM_CHLIST_ENUV        0x80
#define SI1145_PARAM_CHLIST_ENAUX       0x40
#define SI1145_PARAM_CHLIST_ENALSIR     0x20
#define SI1145_PARAM_CHLIST_ENALSVIS    0x10
#define SI1145_PARAM_CHLIST_ENPS1       0x01
#define SI1145_PARAM_CHLIST_ENPS2       0x02
#define SI1145_PARAM_CHLIST_ENPS3       0x04

#define SI1145_PARAM_PSLED12SEL         0x02
#define SI1145_PARAM_PSLED12SEL_PS2NONE 0x00
#define SI1145_PARAM_PSLED12SEL_PS2LED1 0x10
#define SI1145_PARAM_PSLED12SEL_PS2LED2 0x20
#define SI1145_PARAM_PSLED12SEL_PS2LED3 0x40
#define SI1145_PARAM_PSLED12SEL_PS1NONE 0x00
#define SI1145_PARAM_PSLED12SEL_PS1LED1 0x01
#define SI1145_PARAM_PSLED12SEL_PS1LED2 0x02
#define SI1145_PARAM_PSLED12SEL_PS1LED3 0x04

#define SI1145_PARAM_PSLED3SEL          0x03
#define SI1145_PARAM_PSENCODE           0x05
#define SI1145_PARAM_ALSENCODE          0x06

#define SI1145_PARAM_PS1ADCMUX          0x07
#define SI1145_PARAM_PS2ADCMUX          0x08
#define SI1145_PARAM_PS3ADCMUX          0x09
#define SI1145_PARAM_PSADCOUNTER        0x0A
#define SI1145_PARAM_PSADCGAIN          0x0B
#define SI1145_PARAM_PSADCMISC          0x0C
#define SI1145_PARAM_PSADCMISC_RANGE    0x20
#define SI1145_PARAM_PSADCMISC_PSMODE   0x04

#define SI1145_PARAM_ALSIRADCMUX        0x0E
#define SI1145_PARAM_AUXADCMUX          0x0F

#define SI1145_PARAM_ALSVISADCOUNTER    0x10
#define SI1145_PARAM_ALSVISADCGAIN      0x11
#define SI1145_PARAM_ALSVISADCMISC      0x12
#define SI1145_PARAM_ALSVISADCMISC_VISRANGE 0x20

#define SI1145_PARAM_ALSIRADCOUNTER     0x1D
#define SI1145_PARAM_ALSIRADCGAIN       0x1E
#define SI1145_PARAM_ALSIRADCMISC       0x1F
#define SI1145_PARAM_ALSIRADCMISC_RANGE 0x20

#define SI1145_PARAM_ADCCOUNTER_511CLK  0x70

#define SI1145_PARAM_ADCMUX_SMALLIR     0x00
#define SI1145_PARAM_ADCMUX_LARGEIR     0x03

/* SI1145 Registers */
#define SI1145_REG_PARTID               0x00
#define SI1145_REG_REVID                0x01
#define SI1145_REG_SEQID                0x02

#define SI1145_REG_INTCFG               0x03
#define SI1145_REG_INTCFG_INTOE         0x01
#define SI1145_REG_INTCFG_INTMODE       0x02

#define SI1145_REG_IRQEN                0x04
#define SI1145_REG_IRQEN_ALSEVERYSAMPLE 0x01
#define SI1145_REG_IRQEN_PS1EVERYSAMPLE 0x04
#define SI1145_REG_IRQEN_PS2EVERYSAMPLE 0x08
#define SI1145_REG_IRQEN_PS3EVERYSAMPLE 0x10

#define SI1145_REG_IRQMODE1             0x05
#define SI1145_REG_IRQMODE2             0x06

#define SI1145_REG_HWKEY                0x07
#define SI1145_REG_MEASRATE0            0x08
#define SI1145_REG_MEASRATE1            0x09
#define SI1145_REG_PSRATE               0x0A
#define SI1145_REG_PSLED21              0x0F
#define SI1145_REG_PSLED3               0x10
#define SI1145_REG_UCOEFF0              0x13
#define SI1145_REG_UCOEFF1              0x14
#define SI1145_REG_UCOEFF2              0x15
#define SI1145_REG_UCOEFF3              0x16
#define SI1145_REG_PARAMWR              0x17
#define SI1145_REG_COMMAND              0x18
#define SI1145_REG_RESPONSE             0x20
#define SI1145_REG_IRQSTAT              0x21
#define SI1145_REG_IRQSTAT_ALS          0x01

#define SI1145_REG_ALSVISDATA0          0x22
#define SI1145_REG_ALSVISDATA1          0x23
#define SI1145_REG_ALSIRDATA0           0x24
#define SI1145_REG_ALSIRDATA1           0x25
#define SI1145_REG_PS1DATA0             0x26
#define SI1145_REG_PS1DATA1             0x27
#define SI1145_REG_PS2DATA0             0x28
#define SI1145_REG_PS2DATA1             0x29
#define SI1145_REG_PS3DATA0             0x2A
#define SI1145_REG_PS3DATA1             0x2B
#define SI1145_REG_UVINDEX0             0x2C
#define SI1145_REG_UVINDEX1             0x2D
#define SI1145_REG_PARAMRD              0x2E
#define SI1145_REG_CHIPSTAT             0x30

/* SI1145 Constants */
#define SI1145_EXPECTED_PARTID          0x45
#define SI1145_HWKEY_VALUE              0x17

/* UV Coefficient Values (from Adafruit library) */
#define SI1145_UCOEFF0_VALUE            0x29
#define SI1145_UCOEFF1_VALUE            0x89
#define SI1145_UCOEFF2_VALUE            0x02
#define SI1145_UCOEFF3_VALUE            0x00

/*
 * Struct to hold data retrieved from
 * the SI1145.
 */
typedef struct {
	uint16_t uv_index;
	uint16_t ir_light;
	uint16_t visible_light;
	// Proximity sensors
	uint16_t proximity_1;
	uint16_t proximity_2;
	uint16_t proximity_3;
	uint8_t data_valid;
} SI1145_Data_t;


/* Public functions */
BIOME_Error_t SI1145_Init(void);
BIOME_Error_t SI1145_ReadData(SI1145_Data_t *data);
BIOME_Error_t SI1145_TestCommunication(void);

#endif // SI1145_H
