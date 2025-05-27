/*
 * mpu6050.c
 *
 *  Created on: Apr 13, 2025
 *      Author: torik
 */
#include "mpu6050.h"

void MPU6050_Init(MPU6050 *dev, I2C_HandleTypeDef *i2c) {
    dev->i2cHandle = i2c;
    dev->errorCode = 0;
    uint8_t check;
    uint8_t Data;

    HAL_I2C_Mem_Read (dev->i2cHandle, MPU6050_ADDR, 0x75, 1, &check, 1, 100);  // read WHO_AM_I
    if (check == 0x68)  // 0x68 will be returned by the sensor if everything goes well
    {

    	Data = 0;
    	HAL_I2C_Mem_Write(dev->i2cHandle, MPU6050_ADDR, 0x6B, 1, &Data, 1, 100);

        Data = 0x07;
        HAL_I2C_Mem_Write(dev->i2cHandle, MPU6050_ADDR, 0x19, 1, &Data, 1, 100);


        Data = 0x00;
        HAL_I2C_Mem_Write(dev->i2cHandle, MPU6050_ADDR, 0x1C, 1, &Data, 1, 100);

        Data = 0x00;  // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ± 250 ̐/s
        HAL_I2C_Mem_Write(dev->i2cHandle, MPU6050_ADDR, 0x1B, 1, &Data, 1, 100);

        Data = 0x02;
        HAL_I2C_Mem_Write(dev->i2cHandle, MPU6050_ADDR, MPU6050_CONFIG, 1, &Data, 1, 100);
    }
    // Initialize data buffers
    for(int i = 0; i < 3; i++) {
        dev->accel[i] = 0.0f;
        dev->gyro[i] = 0.0f;
    }


    for(int i = 0; i < 14; i++)
    {
    	dev->rxData[i] = 0;
    }

    int32_t accel_sum[3] = {0};
    int32_t gyro_sum[3] = {0};
    uint8_t sample_data[14];

    for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
        if (HAL_I2C_Mem_Read(dev->i2cHandle, MPU6050_ADDR, MPU6050_DATA_START,
                             I2C_MEMADD_SIZE_8BIT, sample_data, 14, 100) != HAL_OK) {
            dev->errorCode |= 0x02;
            return;
        }

        for (int j = 0; j < 3; j++) {
            int16_t accel_raw = (int16_t)((sample_data[j * 2] << 8) | sample_data[j * 2 + 1]);
            int16_t gyro_raw  = (int16_t)((sample_data[8 + j * 2] << 8) | sample_data[9 + j * 2]);

            accel_sum[j] += accel_raw;
            gyro_sum[j]  += gyro_raw;
        }

        HAL_Delay(5);
    }

    for (int i = 0; i < 3; i++) {
        dev->accel_offset[i] = accel_sum[i] / CALIBRATION_SAMPLES;
        dev->gyro_offset[i] = gyro_sum[i] / CALIBRATION_SAMPLES;
    }
}

void MPU6050_ProcessData(MPU6050 *dev)
{

	HAL_I2C_Mem_Read(dev->i2cHandle, MPU6050_ADDR, MPU6050_DATA_START, 1, dev->rxData, 14, 20);
    // Combine bytes and convert to signed integers
    int16_t accelX = (dev->rxData[0] << 8) | dev->rxData[1];
    int16_t accelY = (dev->rxData[2] << 8) | dev->rxData[3];
    int16_t accelZ = (dev->rxData[4] << 8) | dev->rxData[5];

    int16_t gyroX = (dev->rxData[8] << 8) | dev->rxData[9];
    int16_t gyroY = (dev->rxData[10] << 8) | dev->rxData[11];
    int16_t gyroZ = (dev->rxData[12] << 8) | dev->rxData[13];

    // Convert to physical values
    dev->accel[0] = (accelX - dev->accel_offset[0]) / ACCEL_SENSITIVITY_2G;
    dev->accel[1] = (accelY - dev->accel_offset[1]) / ACCEL_SENSITIVITY_2G;
    dev->accel[2] = (accelZ - dev->accel_offset[2]) / ACCEL_SENSITIVITY_2G;

    dev->gyro[0] = (gyroX - dev->gyro_offset[0]) / GYRO_SENSITIVITY_250DPS;
    dev->gyro[1] = (gyroY - dev->gyro_offset[1]) / GYRO_SENSITIVITY_250DPS;
    dev->gyro[2] = (gyroZ - dev->gyro_offset[2]) / GYRO_SENSITIVITY_250DPS;

}
