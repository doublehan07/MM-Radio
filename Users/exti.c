/*
	@Copyright Han ZHANG (DoubleHan) | All rights reserved.
	@Last Modified Time May.24, 2024
*/

/* Includes ------------------------------------------------------------------*/
#include "exti.h"
#include "delay.h"
#include "bk4802.h"
#include "segment_display.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t power_off_state = 0; //State machine status for the power-off function.
uint8_t freq_channel_state = 0; //State machine status for the RX frequency.
uint8_t freq_lock = 0; //Set to non-zero value to lock when the RX frequency needs to be updated.

uint16_t power_off_cnt = 0; //Filter for PWR button in milliseconds, used to stablilize input and reduce finger jitter.
uint16_t ptt_cnt = 0; //Filter for PTT button in milliseconds, used to stablilize input and reduce finger jitter.

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//Overwrites the weak definition in stm32f1xx_hal_gpio.c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{	
	if (GPIO_Pin == PWR_Pin) //If the PWR button is pressed, the device will power off.
	{
		power_off_state = 1;
		power_off_cnt = 1000; //Wait for 1000ms; if the PWR button is still pressed, power off the device.
	}
	
	else if (GPIO_Pin == PTT_Pin) //If the PTT button is pressed, the RX frequency will change.
	{
		if (freq_lock == 0)
		{
			freq_channel_state += 1;
			freq_channel_state %= 12;
			
			freq_lock = 1;
			ptt_cnt = 300; //Wait for 300ms to stabilize input.
		}
	}
}
