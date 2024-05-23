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
//	SysTick->LOAD = 9 * delayTimeUS;				//������װ��ֵ, 72MHZʱ
//	SysTick->CTRL = 0X01;										//ʹ�ܣ�������ʱ�޶����������ⲿʱ��Դ
//	SysTick->VAL = 0;												//���������
//	do
//	{
//		temp = SysTick->CTRL;									//��ȡ��ǰ������ֵ
//	}
//	while((temp&0x01)&&(!(temp&(1<<16))));	//�ȴ�ʱ�䵽��
//	SysTick->CTRL = 0;											//�رռ�����
//	SysTick->VAL = 0;												//��ռ�����
//	
//	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
//}
