/*
 * mpu6050.h
 *
 *  Created on: Apr 13, 2025
 *      Author: torik
 */
#ifndef MPU6050_H
#define MPU6050_H

#include "main.h"
#include "stm32f4xx_hal.h"

#define MPU6050_ADDR         (0x68 << 1)
#define MPU6050_SMPLRT_DIV   0x19
#define MPU6050_CONFIG       0x1A
#define MPU6050_GYRO_CONFIG  0x1B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_INT_PIN_CFG  0x37
#define MPU6050_INT_ENABLE   0x38
#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_DATA_START   0x3B

// Sensitivity factors from datasheet
#define ACCEL_SENSITIVITY_2G     16384.0f  // LSB/g
#define GYRO_SENSITIVITY_250DPS  131.0f    // LSB/°/s
#define CALIBRATION_SAMPLES      1500

typedef struct {
    I2C_HandleTypeDef *i2cHandle;
    float accel[3];         // Acceleration in g (x, y, z)
    float gyro[3];          // Angular rate in °/s (x, y, z)
    uint8_t rxData[14];     // Raw data buffer
    uint8_t errorCode;
    int16_t accel_offset[3]; // Calibration offsets
    int16_t gyro_offset[3];  // Calibration offsets
//    volatile uint8_t dataReady, rxflag;
} MPU6050;

void MPU6050_Init(MPU6050 *dev, I2C_HandleTypeDef *i2c);
//uint8_t MPU6050_Read_DMA(MPU6050 *dev);
void MPU6050_ProcessData(MPU6050 *dev);
void MPU6050_Calibrate(MPU6050 *dev);
#endif // MPU6050_H
