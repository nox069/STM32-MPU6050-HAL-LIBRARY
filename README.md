# MPU6050 STM32 HAL Library

A lightweight STM32 HAL-based library for interfacing with the **MPU6050** 6-axis accelerometer and gyroscope sensor. This library supports initialization, calibration, and real-time data acquisition using STM32 microcontrollers and the HAL library.

## Features

- I2C-based communication
- Initialization with built-in sensor verification (`WHO_AM_I`)
- Automatic offset calibration for accelerometer and gyroscope
- Real-time data processing for:
  - Acceleration in **g** (x, y, z)
  - Angular velocity in **°/s** (x, y, z)

---

## Getting Started

### 1. Include the header
```c
#include "mpu6050.h"
MPU6050 imu;
```

### 2. Initialize the sensor (typically in main())
```C
MPU6050_Init(&imu, &hi2c3); // Replace 'hi2c3' with your configured I2C handle
```

### 3.  Process sensor data (in the main loop)
```c
MPU6050_ProcessData(&imu);

// Access processed data
float ax = imu.accel[0]; // Acceleration X in g
float ay = imu.accel[1]; // Acceleration Y in g
float az = imu.accel[2]; // Acceleration Z in g

float gx = imu.gyro[0];  // Gyroscope X in °/s
float gy = imu.gyro[1];  // Gyroscope Y in °/s
float gz = imu.gyro[2];  // Gyroscope Z in °/s
```
## Configuration
- I2C Address: 0x68 (default)
- Accelerometer Range: ±2g
- Gyroscope Range: ±250°/s
- Calibration: Done automatically on initialization with 1500 samples

## Requirements
- STM32 HAL library (tested with STM32F4 series)
- Compatible MPU6050 breakout board
- Configured I2C peripheral

## File Structure
- mpu6050.h — Header file with definitions and function prototypes
- mpu6050.c — Source file implementing the functionality

