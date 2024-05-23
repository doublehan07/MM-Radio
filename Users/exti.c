/*
	@Copyright Han ZHANG (DoubleHan) | All rights reserved.
	@Last Modified Time May.24, 2024
*/

/* Includes ------------------------------------------------------------------*/
#include "exti.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//Overwrite weak definition in stm32f1xx_hal_gpio.c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == PWR_Pin) //If PWR is pressed, power off.
	{
		if (HAL_GPIO_ReadPin(PWR_GPIO_Port, PWR_Pin) == GPIO_PIN_SET)
		{
			HAL_GPIO_WritePin(PA0_GPIO_Port, PA0_Pin, GPIO_PIN_RESET); //Disable CE. Turn off LED1.
		}
	}
	
	else if (GPIO_Pin == PTT_Pin)
	{
		
	}
}
