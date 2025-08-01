################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BME68X/Src/bme68x.c 

OBJS += \
./Drivers/BME68X/Src/bme68x.o 

C_DEPS += \
./Drivers/BME68X/Src/bme68x.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BME68X/Src/%.o Drivers/BME68X/Src/%.su Drivers/BME68X/Src/%.cyclo: ../Drivers/BME68X/Src/%.c Drivers/BME68X/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BME68X-2f-Src

clean-Drivers-2f-BME68X-2f-Src:
	-$(RM) ./Drivers/BME68X/Src/bme68x.cyclo ./Drivers/BME68X/Src/bme68x.d ./Drivers/BME68X/Src/bme68x.o ./Drivers/BME68X/Src/bme68x.su

.PHONY: clean-Drivers-2f-BME68X-2f-Src

