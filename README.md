# STM32H745-DISCO-TEST
Repo for code generated for the STM32H745 Disco board and used while testing I2C/UART/SPI peripherals.

Contains my 'driver' code for each of the sensors. Sensors can be temporarily enabled/disabled with `#define` statements in `main.h` of the form `USE_SENSOR`, e.g. `USE_BME680` to enable everything related to the BME680.
For now, I am only using one I2C bus on the STM32, so this will be helpful in testing the servos (I'm hoping gridfin control can be on an I2C bus separate from the sensors).

The BME680 driver wraps Bosch's existing C library in BIOME-compatible functions, while the rest of the sensors are essentially adapting Adafruit's C++ Arduino libraries to C and the STM32 platform.

I think a bunch of the sensor-specific static i2c read/write functions could be consolidated into a single file

