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
static __IO uint16_t display_num_H = 0; //Extracts and displays the first 4 digits, e.g. 439.250 => 439.
static __IO uint16_t display_num_L = 0; //Extracts and displays the last 4 digits, e.g. 439.250 => 250.
uint8_t code[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E}; //Code for displaying 0-F on a single LED segment display.

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//Control logic fot the 74HC595 chip to display the value 'num'.
//seg = {0x08, 0x04, 0x02, 0x01}
//e.g. to display '2345'
//Use writeByteH(2, 0x08) to display 2xxx.
//Use writeByteH(3, 0x04) to display x3xx.
//Use writeByteH(4, 0x02) to display xx4x.
//Use writeByteH(5, 0x01) to display xxx5.
//The 74HC595 chip can handle only one digit at a time, so '2345' cannot be displayed simultaneously.
//To display '2345', call the upper 4 functions every 1 millisecond.
//In other words, scan each digit sequentially to create the illusion of a simultaneous display.
void WriteByteH(uint8_t num, uint8_t seg)
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

void WriteByteL(uint8_t num, uint8_t seg)
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

void SetNumH(uint16_t num)
{
	display_num_H = num;
}

void SetNumL(uint16_t num)
{
	display_num_L = num;
}

//Usage: Call DBH_SetNum(439, 250) to display 439 on segment display H and 250 on segment display L.
void DBH_SetNum(uint16_t num_H, uint16_t num_L)
{
	SetNumH(num_H);
	SetNumL(num_L);
}

//The 'state' will alternate between 0 and 3 every 1 millisecond, as scheduled in the SysTick Handler.
void DBH_DisplayNumH(uint8_t state)
{
	switch (state)
	{
		case 0: 
			if (display_num_H/1000 != 0) //If the first digit of display_num_H is 0, skip it (e.g. display '439' instead of '0439').
				WriteByteH(code[display_num_H/1000],0x08); 
			break;
		case 1: WriteByteH(code[display_num_H%1000/100],0x04); break;
		case 2: WriteByteH(code[display_num_H%100/10],0x02); break;
		case 3: WriteByteH(code[display_num_H%100%10],0x01); break;
		default: break;
	}
}

//The 'state' will alternate between 0 and 3 every 1 millisecond, as scheduled in the SysTick Handler.
void DBH_DisplayNumL(uint8_t state)
{
	switch (state)
	{
		case 0: 
			if (display_num_L/1000 != 0) //If the first digit of display_num_L is 0, skip it (e.g. display '250' instead of '0250').
				WriteByteL(code[display_num_L/1000],0x08); 
			break;
		case 1: WriteByteL(code[display_num_L%1000/100],0x04); break;
		case 2: WriteByteL(code[display_num_L%100/10],0x02); break;
		case 3: WriteByteL(code[display_num_L%100%10],0x01); break;
		default: break;
	}
}
