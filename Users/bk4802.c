/*
	@Copyright Han ZHANG (DoubleHan) | All rights reserved.
	@BK4802 communication protocol credit to BG7QKU
	@For more details, please visit https://github.com/BG7QKU
	@Last Modified Time May.24, 2024
*/

/* Includes ------------------------------------------------------------------*/
#include "bk4802.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SCL_HIGH		HAL_GPIO_WritePin(GPIOA, CLK_Pin, GPIO_PIN_SET)
#define SCL_LOW 		HAL_GPIO_WritePin(GPIOA, CLK_Pin, GPIO_PIN_RESET)

#define SDA_HIGH		HAL_GPIO_WritePin(GPIOA, DATA_Pin, GPIO_PIN_SET)
#define SDA_LOW			HAL_GPIO_WritePin(GPIOA, DATA_Pin, GPIO_PIN_RESET)

#define HIGHBYTE(x) ((x) >> 8)
#define LOWBYTE(x)	((x) & 0xFF)

/* Private variables ---------------------------------------------------------*/
static __IO uint16_t rx_reg[]={ 
0x0300, //reg_addr=4, Set to RX Mode
0x0C04, //reg_addr=5, default=0x0C04(RX)
0xF140, //reg_addr=6, default=0xF140
0xED00, //reg_addr=7, RX IF Gain=21dB
0x17E0, //reg_addr=8, TX Settings
0xE0E0, //reg_addr=9, T/R Control by DSP, PLL in RX Mode, RF Switch in RX Mode
0x8543, //reg_addr=10, default=0x8543
0x0700, //reg_addr=11, default=0x0700
0xA066, //reg_addr=12, default=0xA066
0xFFFF, //reg_addr=13, default=0xFFFF
0xFFE0, //reg_addr=14, TX Settings
0x07A0, //reg_addr=15, TX Settings
0x9E3C, //reg_addr=16, TX Settings
0x1F00, //reg_addr=17, TX Settings
0xD1C7, //reg_addr=18, Speaker on if ON conditions last for 0-100ms, Speaker off if OFF conditions last for 7-800ms
0x200F, //reg_addr=19, CIC Filter Gain=0dB, Output Amplitude of FM Demodulator=10, RX Audio Volume=1111
0x01FF, //reg_addr=20, Enable AFC
0xE000, //reg_addr=21, default=0xE000
0x0300  //reg_addr=22, Ex-noise Coefficient for Speaker OFF Condition=00, RSSI Threshold for Speaker ON Condition=00000000
//0xFF=speaker always off (during test)
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//GPIO simulation for I2C protocol.
//For some reasons, calling HAL_I2C_Master_Transmit() will cause a fatal error.
//Instead of using HAL_I2C, we use GPIO simulation of I2C due to issues above.
void IICStart()
{
  SCL_HIGH;
  SDA_HIGH;
  SDA_LOW;
  SCL_LOW;
}

void IICStop()
{
  SCL_LOW;
  SDA_LOW;
  SCL_HIGH;
  SDA_HIGH;
}

void WriteIICByte(uint8_t data)
{
	for(uint8_t i=0; i<8; i++)
	{
		if(data & 0x80)
			SDA_HIGH;
		else
			SDA_LOW;
		SCL_HIGH;
		SCL_LOW;
		data<<=1;
	}
	SDA_HIGH;
	SCL_HIGH;
	SCL_LOW;
}

//Send control command to BK4802.
//IC_addr is always set to 0x90.
//reg_addr according to datasheet, ranges from 0 to 33.
//data_h is the higher byte of the data
//data_l is the lower byte of the data.
void DBH_WriteTo4802(uint8_t IC_addr, uint8_t reg_addr, uint8_t data_h, uint8_t data_l)
{
	IICStart();
	WriteIICByte(IC_addr);
	WriteIICByte(reg_addr); 
	WriteIICByte(data_h); 
	WriteIICByte(data_l); 
	IICStop();
	DBH_DelayMS(1);
}

void DBH_SetRXMode(void)
{
	for(uint8_t i=4; i<=22; i++)
	{
		DBH_WriteTo4802(IC_ADDR, i, HIGHBYTE(rx_reg[i-4]), LOWBYTE(rx_reg[i-4]));
	}
}

//The fracn can be caculated using fracn.py.
void DBH_SetRXFreq(uint16_t fracn_h, uint16_t fracn_l)
{
	DBH_WriteTo4802(IC_ADDR, 2, HIGHBYTE(0x0000), LOWBYTE(0x0000));
	DBH_WriteTo4802(IC_ADDR, 1, HIGHBYTE(fracn_l), LOWBYTE(fracn_l));
	DBH_WriteTo4802(IC_ADDR, 0, HIGHBYTE(fracn_h),LOWBYTE(fracn_h));
}
