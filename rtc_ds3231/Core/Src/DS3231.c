/*
 * DS3231.c
 *
 *  Created on: Oct 1, 2024
 *      Author: Pheonix
 */


#include "DS3231.h"
#include "main.h"
#include <stdio.h>
#include "retarget.h"
#include "stm32g0xx_hal_i2c.h"

#define Trials 4
#define Timeout 100

#define hi2c &hi2c1

#define DevAddress 0x68<<1


extern I2C_HandleTypeDef hi2c1;

int bcd_to_bin(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}



void rtc_init(){
	HAL_StatusTypeDef test= HAL_I2C_IsDeviceReady(&hi2c1, DevAddress, Trials, Timeout);
	  if(test==HAL_OK){
		  printf("\r\n device is ready \r\n");

	  }
	  else{
		  printf("\r\n device is not ready \r\n");
	  }

}


// Function to convert an integer to BCD format
uint8_t to_bcd(int value) {
    return ((value / 10) << 4) | (value % 10);
}

void rtc_set_time(int hour, int minute, int second) {
    // Validate time values
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        printf("Invalid time values\n");
        return; // Handle invalid values appropriately
    }

    // Prepare data to be sent to the DS3231 (BCD format)
    uint8_t pData[3];
    pData[0] = to_bcd(second);  // Convert seconds to BCD
    pData[1] = to_bcd(minute);  // Convert minutes to BCD
    pData[2] = to_bcd(hour);    // Convert hours to BCD

    // Write time to the RTC
    if (HAL_I2C_Mem_Write(&hi2c1, DevAddress, 0x00, I2C_MEMADD_SIZE_8BIT, pData, sizeof(pData), HAL_MAX_DELAY) != HAL_OK) {
        // Handle error
        printf("Error writing to RTC\n");
    }
}



void rtc_get_time(int *hour, int *minute, int *second) {
    uint8_t pData[3];

    // Read the current time from the DS3231
    if (HAL_I2C_Mem_Read(&hi2c1, DevAddress, 0x00, I2C_MEMADD_SIZE_8BIT, pData, sizeof(pData), HAL_MAX_DELAY) != HAL_OK) {
        printf("Error reading from RTC\n");
        return;
    }

    // Convert BCD to binary
    *second = bcd_to_bin(pData[0] & 0x7F); // Mask to ensure valid range
    *minute = bcd_to_bin(pData[1] & 0x7F);
    *hour = bcd_to_bin(pData[2] & 0x3F);

    // Validate read values
    if (*second > 59 || *minute > 59 || *hour > 23) {
        printf("Invalid time read from RTC\n");
    }
}
