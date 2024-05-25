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
static __IO uint32_t s_tick; //s_tick is set for the user's delay function.

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//DBH_DexTick() should be called every 1 ms in the SysTick interrupt function.
void DBH_DecTick(void)
{
	if (s_tick != 0)
	{
		s_tick--;
	}
}

uint32_t GetTick(void)
{
	return s_tick;
}

void SetTick(uint32_t set_tick)
{
	s_tick = set_tick;
}

//Empty loop to create a delay of delay_time_ms milliseconds.
void DBH_DelayMS(uint32_t delay_time_ms)
{
	SetTick(delay_time_ms);
	while (GetTick() != 0);
}
