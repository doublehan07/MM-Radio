/*
	@Copyright Han ZHANG (DoubleHan) | All rights reserved.
	@Last Modified Time May.24, 2024
*/

/* Includes ------------------------------------------------------------------*/
#include "segment_display.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define DIO1_HIGH		HAL_GPIO_WritePin(DIO1_GPIO_Port, DIO1_Pin, GPIO_PIN_SET)
#define DIO1_LOW 		HAL_GPIO_WritePin(DIO1_GPIO_Port, DIO1_Pin, GPIO_PIN_RESET)

#define SCLK1_HIGH		HAL_GPIO_WritePin(SCLK1_GPIO_Port, SCLK1_Pin, GPIO_PIN_SET)
#define SCLK1_LOW 		HAL_GPIO_WritePin(SCLK1_GPIO_Port, SCLK1_Pin, GPIO_PIN_RESET)

#define RCLK1_HIGH		HAL_GPIO_WritePin(RCLK1_GPIO_Port, RCLK1_Pin, GPIO_PIN_SET)
#define RCLK1_LOW 		HAL_GPIO_WritePin(RCLK1_GPIO_Port, RCLK1_Pin, GPIO_PIN_RESET)

#define DIO2_HIGH		HAL_GPIO_WritePin(DIO2_GPIO_Port, DIO2_Pin, GPIO_PIN_SET)
#define DIO2_LOW 		HAL_GPIO_WritePin(DIO2_GPIO_Port, DIO2_Pin, GPIO_PIN_RESET)

#define SCLK2_HIGH		HAL_GPIO_WritePin(SCLK2_GPIO_Port, SCLK2_Pin, GPIO_PIN_SET)
#define SCLK2_LOW 		HAL_GPIO_WritePin(SCLK2_GPIO_Port, SCLK2_Pin, GPIO_PIN_RESET)

#define RCLK2_HIGH		HAL_GPIO_WritePin(RCLK2_GPIO_Port, RCLK2_Pin, GPIO_PIN_SET)
#define RCLK2_LOW 		HAL_GPIO_WritePin(RCLK2_GPIO_Port, RCLK2_Pin, GPIO_PIN_RESET)

/* Private variables ---------------------------------------------------------*/
static __IO uint16_t display_num_H = 0;
static __IO uint16_t display_num_L = 0;
uint8_t code[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E}; //0-F

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void writeByteH(uint8_t num, uint8_t seg)
{
	for(uint8_t i=0; i<8; i++)
	{
		((num<<i)&0x80) ? DIO1_HIGH : DIO1_LOW;
		SCLK1_HIGH;
		SCLK1_LOW;
	}
 
	for(uint8_t i=0; i<8; i++)
	{
		((seg<<i)&0x80) ? DIO1_HIGH : DIO1_LOW;
		SCLK1_HIGH;
		SCLK1_LOW;
 
	}
	RCLK1_HIGH;
	RCLK1_LOW;
}

void writeByteL(uint8_t num, uint8_t seg)
{
	for(uint8_t i=0; i<8; i++)
	{
		((num<<i)&0x80) ? DIO2_HIGH : DIO2_LOW;
		SCLK2_HIGH;
		SCLK2_LOW;
	}
 
	for(uint8_t i=0; i<8; i++)
	{
		((seg<<i)&0x80) ? DIO2_HIGH : DIO2_LOW;
		SCLK2_HIGH;
		SCLK2_LOW;
 
	}
	RCLK2_HIGH;
	RCLK2_LOW;
}

void setNumH(uint16_t num)
{
	display_num_H = num;
}

void setNumL(uint16_t num)
{
	display_num_L = num;
}

void DBH_SetNum(uint16_t numH, uint16_t numL)
{
	setNumH(numH);
	setNumL(numL);
}

void DBH_DisplayNumH(uint8_t state)
{
	switch (state)
	{
		case 0: 
			if (display_num_H/1000 != 0)
				writeByteH(code[display_num_H/1000],0x08); 
			break;
		case 1: writeByteH(code[display_num_H%1000/100],0x04); break;
		case 2: writeByteH(code[display_num_H%100/10],0x02); break;
		case 3: writeByteH(code[display_num_H%100%10],0x01); break;
		default: break;
	}
}

void DBH_DisplayNumL(uint8_t state)
{
	switch (state)
	{
		case 0: 
			if (display_num_L/1000 != 0)
				writeByteL(code[display_num_L/1000],0x08); 
			break;
		case 1: writeByteL(code[display_num_L%1000/100],0x04); break;
		case 2: writeByteL(code[display_num_L%100/10],0x02); break;
		case 3: writeByteL(code[display_num_L%100%10],0x01); break;
		default: break;
	}
}
