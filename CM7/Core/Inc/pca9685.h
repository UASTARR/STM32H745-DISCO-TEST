#ifndef PCA9685_H
#define PCA9685_H

#include "main.h"
#include "error.h"
#include "../../Drivers/stm32-hal-pca9685/pca9685.h"

BIOME_Error_t PCA9685_Init(void);
BIOME_Error_t PCA9685_Write(void);
BIOME_Error_t PCA9685_Rotate180(void);
BIOME_Error_t PCA9685_TestRotation(void);
BIOME_Error_t PCA9685_CheckSleeping(void);

#endif // PCA9685_H

