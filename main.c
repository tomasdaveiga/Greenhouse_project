//
// Created by tomas on 28/04/23.
//

#include "project.h"
#include <stdio.h>

#define OPT3001_ADDR 0x44
#define OPT3001_REG_RESULT 0x00

#define LSM6DS3_ADDR 0x6B // I2C address of the LSM6DS3 sensor
#define LSM6DS3_REG_CTRL1_XL 0x10 // Register address for the control register 1 for the accelerometer
#define LSM6DS3_REG_OUTX_L_XL 0x28 // Register address for the low byte of the x-axis data of the accelerometer

#define SI7020_I2C_ADDR    0x40 // I2C address of the Si7020 sensor
#define SI7020_REG_TEMP    0xE3 // Register address for temperature data
#define SI7020_REG_HUMID   0xE5 // Register address for humidity data

float read_temperature()
{
    uint8_t tx_data[1], rx_data[2];
    uint16_t result;

    /* Send the register address to the sensor using I2C */
    tx_data[0] = SI7020_REG_TEMP;
    I2C_MasterWriteBuf(SI7020_I2C_ADDR, tx_data, 1, I2C_MODE_COMPLETE_XFER);

    /* Read the data from the sensor using I2C */
    I2C_MasterReadBuf(SI7020_I2C_ADDR, rx_data, 2, I2C_MODE_COMPLETE_XFER);

    /* Combine the received data bytes into a 16-bit integer */
    result = (uint16_t)(rx_data[1] << 8 | rx_data[0]);

    /* Convert the raw sensor data to degrees Celsius */
    float temperature = ((175.72 * result) / 65536) - 46.85;

    return temperature;
}

float read_humidity()
{
    uint8_t tx_data[1], rx_data[2];
    uint16_t result;

    /* Send the register address to the sensor using I2C */
    tx_data[0] = SI7020_REG_HUMID;
    I2C_MasterWriteBuf(SI7020_I2C_ADDR, tx_data, 1, I2C_MODE_COMPLETE_XFER);

    /* Read the data from the sensor using I2C */
    I2C_MasterReadBuf(SI7020_I2C_ADDR, rx_data, 2, I2C_MODE_COMPLETE_XFER);

    /* Combine the received data bytes into a 16-bit integer */
    result = (uint16_t)(rx_data[1] << 8 | rx_data[0]);

    /* Convert the raw sensor data to relative humidity in percent */
    float humidity = ((125 * result) / 65536) - 6;

    return humidity;
}


int16_t read_sensor_data(uint8_t reg_addr)
{
    uint8_t tx_data[1], rx_data[2];
    int16_t result;

    /* Send the register address to the sensor using I2C */
    tx_data[0] = reg_addr;
    I2C_MasterWriteBuf(LSM6DS3_ADDR, tx_data, 1, I2C_MODE_COMPLETE_XFER);

    /* Read the data from the sensor using I2C */
    I2C_MasterReadBuf(LSM6DS3_ADDR, rx_data, 2, I2C_MODE_COMPLETE_XFER);

    /* Combine the received data bytes into a signed 16-bit integer */
    result = (int16_t)(rx_data[1] << 8 | rx_data[0]);

    return result;
}

int main(void){
    CyGlobalIntEnable;

    /* Light Sensor */
    uint8_t opt_data[2];
    uint16_t lux;

    /* Accelerometer */
    int16_t accel_x, accel_y, accel_z;

    /* Temperature and Humidity sensor */
    float temperature, humidity;

    /* Start I2C */
    I2C_Start();
    CyDelay(100);

    /* Configure the sensors */
    I2C_MasterWriteBuf(OPT3001_ADDR, (uint8_t []){0x01, 0xCE}, 2, I2C_MODE_COMPLETE_XFER);
    I2C_MasterWriteBuf(LSM6DS3_ADDR, (uint8_t []){LSM6DS3_REG_CTRL1_XL, 0x60}, 2, I2C_MODE_COMPLETE_XFER);

    uint8_t ctrl1_xl_data[2] = {0x10, 0x60}, ctrl2_g_data[2] = {0x11, 0x60}; // Address and data for CTRL1_XL and CTRL2_G register
    I2C_MasterWriteBuf(LSM6DS3_ADDR, ctrl1_xl_data, 2, I2C_MODE_COMPLETE_XFER);
    I2C_MasterWriteBuf(LSM6DS3_ADDR, ctrl2_g_data, 2, I2C_MODE_COMPLETE_XFER);


    for(;;){
        // Read Light Sensor data
        I2C_MasterWriteBuf(OPT3001_ADDR, (uint8_t []){OPT3001_REG_RESULT}, 1, I2C_MODE_NO_STOP);
        I2C_MasterReadBuf(OPT3001_ADDR, opt_data, 2, I2C_MODE_REPEAT_START);
        lux = ((opt_data[0] << 8) | opt_data[1]) * 0.01;

        /* Print the measured lux value to the console */
        printf("Lux: %d\r\n", lux);

        // Read Accelerometer
        accel_x = read_sensor_data(LSM6DS3_REG_OUTX_L_XL);
        accel_y = read_sensor_data(LSM6DS3_REG_OUTX_L_XL + 2);
        accel_z = read_sensor_data(LSM6DS3_REG_OUTX_L_XL + 4);

        /* Read the temperature and humidity data from the sensor */
        temperature = read_temperature();
        humidity = read_humidity();

        /* Print the measured data to the console */
        printf("Temperature: %.2f C, Humidity: %.2f %%RH\r\n", temperature, humidity);


        /* Wait for some time before taking the next measurement */
        CyDelay(1000);
    }

}