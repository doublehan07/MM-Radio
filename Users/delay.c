/*
	@Copyright Han ZHANG (DoubleHan) | All rights reserved.
	@Last Modified Time Apr.29, 2023
*/

/* Includes ------------------------------------------------------------------*/
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t sTick;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//Call for Systick IRQ Handler
//Overwrite weak definition in stm32f1xx_hal.c

void DBH_DecTick(void)
{
	if (sTick != 0)
	{
		sTick--;
	}
}

uint32_t GetTick(void)
{
	return sTick;
}

void SetTick(uint32_t setTick)
{
	sTick = setTick;
}

void DBH_DelayMS(uint32_t delayTimeMS)
{
	SetTick(delayTimeMS);
	while (GetTick() != 0);
}

//void DBH_DelayUS(uint16_t delayTimeUS)
//{
//	uint32_t temp;
//	if (delayTimeUS > 1000)
//		return;
//	
//	SysTick->LOAD = 9 * delayTimeUS;				//设置重装数值, 72MHZ时
//	SysTick->CTRL = 0X01;										//使能，减到零时无动作，采用外部时钟源
//	SysTick->VAL = 0;												//清零计数器
//	do
//	{
//		temp = SysTick->CTRL;									//读取当前倒计数值
//	}
//	while((temp&0x01)&&(!(temp&(1<<16))));	//等待时间到达
//	SysTick->CTRL = 0;											//关闭计数器
//	SysTick->VAL = 0;												//清空计数器
//	
//	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
//}
