/*
 * @file error.h
 * @author Miro Straszynski
 * @brief Definitions of error codes for BIOME-III *
 * 
 */
#ifndef ERROR_H
#define ERROR_H


/*
 * Error codes used by BIOME-III. 
 * I'm currently using three types of error codes, though this
 * could be expanded if needed. 
 * Current types are SYS / system, SENS / sensor, COMM / communication.
 * A system error should be something specific to the system we have
 * developed. A communication error should be related to communication
 * with sensors or other devices, and a sensor error should be something
 * specific to the sensor's operation.
 *
 * This can be expanded to up to 15 more error types, with
 * 16^3 - 1 error codes per type.
 */ 
typedef enum {
    // Success codes
    SYS_OK = 0,
    SENS_OK = 0,
    COMM_OK = 0,

    // System errors
    SYS_ERR_NULL_PTR = 0x1001,
    SYS_ERR_INV_ARG = 0x1002,

    // Sensor errors
    SENS_ERR_INV_ID = 0x2001,
    SENS_ERR_NOT_RDY = 0x2002, // Attempted to use sensor while not ready/initialized.
	SENS_ERR_INIT = 0x2003, // An error occurred while initializing the sensor.
	SENS_ERR_CONF = 0x2004, // An error occurred while configuring the sensor.
    SENS_ERR_RST = 0x2005, // Error occurred while resetting the sensor.
	SENS_ERR_TIMEOUT = 0x2006, // A timeout occurred while reading the sensor.
    SENS_ERR_READ = 0x2007, // An error occurred while reading the sensor.
    
    // Communication errors
    COMM_ERR_I2C_BUSY = 0x3001,
    COMM_ERR_SPI_BUSY = 0x3002,
    

} BIOME_Error_t;

#endif // ERROR_H
