/*
 * mpu6050.c
 *
 *  Created on: Sep 29, 2024
 *      Author: Daniel
 */
#include "mpu6050.h"
#include "main.h"
#include <stdio.h>
#include "retarget.h"
#include "stm32g0xx_hal_i2c.h"

#define Trials 4
#define Timeout 100

#define hi2c &hi2c1


extern I2C_HandleTypeDef hi2c1;

void mpu6050_init(){
	HAL_StatusTypeDef mpu_ready = HAL_I2C_IsDeviceReady(hi2c, DEVICE_ADDRESS, Trials, Timeout);

	if(mpu_ready == HAL_OK){
		printf("\r\nMPU6050 device is ready for communication\r\n");
	}
	else{
		printf("\r\nMPU6050 device is not responding\r\n");
	}

	uint8_t temp = FS_GYRO_500;

	HAL_StatusTypeDef mpu_reg_wr = HAL_I2C_Mem_Write(hi2c, DEVICE_ADDRESS, REG_CONFIG_GYRO, 1, &temp, 1, Timeout);
	if(mpu_reg_wr == HAL_OK){
		printf("\r\nGyroscope configuration successful\r\n");
	}
	else{
		printf("\r\nFailed to configure gyroscope\r\n");
	}

	temp = FS_ACC_4G;
	mpu_reg_wr = HAL_I2C_Mem_Write(hi2c, DEVICE_ADDRESS, REG_CONFIG_ACC, 1, &temp, 1, Timeout);
	if(mpu_reg_wr == HAL_OK){
		printf("\r\nAccelerometer configuration successful\r\n");
	}
	else{
		printf("\r\nFailed to configure accelerometer\r\n");
	}

	temp = 0;
	mpu_reg_wr = HAL_I2C_Mem_Write(hi2c, DEVICE_ADDRESS, REG_USR_CTRL, 1, &temp, 1, Timeout);
	if(mpu_reg_wr == HAL_OK){
		printf("\r\nExited sleep mode\r\n");
	}
	else{
		printf("\r\nFailed to exit sleep mode\r\n");
	}
}

void mpu6050_read(){
	uint8_t data[6]; // Read 6 bytes: X, Y, Z
	int16_t x_acc, y_acc, z_acc;

	// Read 6 bytes starting from REG_DATA
	HAL_I2C_Mem_Read(hi2c, (DEVICE_ADDRESS)+1, REG_DATA, 1, data, 6, Timeout);

	// Combine high and low bytes for X, Y, Z
	x_acc = ((int16_t)data[0] << 8) | data[1];
	y_acc = ((int16_t)data[2] << 8) | data[3];
	z_acc = ((int16_t)data[4] << 8) | data[5];

	// Print the results
	printf("\r\nX-axis acceleration: %d\r\n", x_acc);
	printf("Y-axis acceleration: %d\r\n", y_acc);
	printf("Z-axis acceleration: %d\r\n", z_acc);
}
