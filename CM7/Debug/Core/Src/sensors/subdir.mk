################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/sensors/bme680.c \
../Core/Src/sensors/mmc56x3.c \
../Core/Src/sensors/mpl3115a2.c \
../Core/Src/sensors/sgp30.c \
../Core/Src/sensors/si1145.c 

OBJS += \
./Core/Src/sensors/bme680.o \
./Core/Src/sensors/mmc56x3.o \
./Core/Src/sensors/mpl3115a2.o \
./Core/Src/sensors/sgp30.o \
./Core/Src/sensors/si1145.o 

C_DEPS += \
./Core/Src/sensors/bme680.d \
./Core/Src/sensors/mmc56x3.d \
./Core/Src/sensors/mpl3115a2.d \
./Core/Src/sensors/sgp30.d \
./Core/Src/sensors/si1145.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/sensors/%.o Core/Src/sensors/%.su Core/Src/sensors/%.cyclo: ../Core/Src/sensors/%.c Core/Src/sensors/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-sensors

clean-Core-2f-Src-2f-sensors:
	-$(RM) ./Core/Src/sensors/bme680.cyclo ./Core/Src/sensors/bme680.d ./Core/Src/sensors/bme680.o ./Core/Src/sensors/bme680.su ./Core/Src/sensors/mmc56x3.cyclo ./Core/Src/sensors/mmc56x3.d ./Core/Src/sensors/mmc56x3.o ./Core/Src/sensors/mmc56x3.su ./Core/Src/sensors/mpl3115a2.cyclo ./Core/Src/sensors/mpl3115a2.d ./Core/Src/sensors/mpl3115a2.o ./Core/Src/sensors/mpl3115a2.su ./Core/Src/sensors/sgp30.cyclo ./Core/Src/sensors/sgp30.d ./Core/Src/sensors/sgp30.o ./Core/Src/sensors/sgp30.su ./Core/Src/sensors/si1145.cyclo ./Core/Src/sensors/si1145.d ./Core/Src/sensors/si1145.o ./Core/Src/sensors/si1145.su

.PHONY: clean-Core-2f-Src-2f-sensors

