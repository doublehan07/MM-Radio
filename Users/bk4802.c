/*
	@Copyright Han ZHANG (DoubleHan) | All rights reserved.
	@BK4802 communication protocol credit to BG7QKU
	@For more details, please visit https://github.com/BG7QKU
	@Last Modified Time May.24, 2024
*/

/* Includes ------------------------------------------------------------------*/
#include "bk4802.h"
//#include "i2c.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define IC_ADDR			0x90

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t rx_reg[]={ 
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
void IIC_Start()
{
  HAL_GPIO_WritePin(GPIOA, CLK_Pin, GPIO_PIN_SET);// SCL = high;		
  HAL_GPIO_WritePin(GPIOA, DATA_Pin, GPIO_PIN_SET);// SDA = high;
  HAL_GPIO_WritePin(GPIOA, DATA_Pin, GPIO_PIN_RESET); //SDA = low;
  HAL_GPIO_WritePin(GPIOA, CLK_Pin, GPIO_PIN_RESET); //SCL = low;
}

void IIC_Stop()
{
  HAL_GPIO_WritePin(GPIOA, CLK_Pin, GPIO_PIN_RESET); // SCL = low;
  HAL_GPIO_WritePin(GPIOA, DATA_Pin, GPIO_PIN_RESET); // SDA = low;
  HAL_GPIO_WritePin(GPIOA, CLK_Pin, GPIO_PIN_SET);// SCL = high;
  HAL_GPIO_WritePin(GPIOA, DATA_Pin, GPIO_PIN_SET);//  SDA = high;
}

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	for(uint8_t i=0; i<8; i++)
	{
		if(IIC_Byte & 0x80)
			HAL_GPIO_WritePin(GPIOA, DATA_Pin, GPIO_PIN_SET);//SDA=high;
		else
			HAL_GPIO_WritePin(GPIOA, DATA_Pin, GPIO_PIN_RESET); //SDA=low;
		HAL_GPIO_WritePin(GPIOA, CLK_Pin, GPIO_PIN_SET);// SCL=high;
		HAL_GPIO_WritePin(GPIOA, CLK_Pin, GPIO_PIN_RESET); //SCL=low;
		IIC_Byte<<=1;
	}
	HAL_GPIO_WritePin(GPIOA, DATA_Pin, GPIO_PIN_SET);// SDA=1;
	HAL_GPIO_WritePin(GPIOA, CLK_Pin, GPIO_PIN_SET);//SCL=1;
	HAL_GPIO_WritePin(GPIOA, CLK_Pin, GPIO_PIN_RESET); //SCL=0;
}

void writing(unsigned char ICaddforwrite,unsigned char hadd,unsigned char hdata,unsigned char ldata)//WRITE TO BK4802N
{
	IIC_Start();
	Write_IIC_Byte(ICaddforwrite);
	Write_IIC_Byte(hadd); 
	Write_IIC_Byte(hdata); 
	Write_IIC_Byte(ldata); 
	IIC_Stop();
	DBH_DelayMS(1);
}
	 
void writeToBK4802(uint8_t IC_addr, uint8_t reg_addr, uint8_t data_h, uint8_t data_l) //Write to BK4802
{
	uint8_t tx_data[3];
	tx_data[0] = reg_addr;
	tx_data[1] = data_h;
	tx_data[2] = data_l;
	//HAL_I2C_Master_Transmit(&hi2c1, IC_addr, tx_data, 3, 1000);
}

void DBH_BK4802_Init(void)
{
	writeToBK4802(IC_ADDR, 23, 0x60, 0xFF);
	//reg_addr=23
	//B15: 0-Disable Power Saving
	//B14-B13: 11(3)-RX Active Period in Power Saving Mode=192ms
	//B12-B11: 00(0)-Sleep Period in Power Saving Mode=384ms
	//B08: 0-PIN7 is CALL, PIN24 is RSSI
	//B07-B00: 0xFF(255)-Ex-noise Threshold for Speaker ON Condition
	//0x00=speaker always off (during test)
}

void DBH_SetToRxMode(void)
{
	for (uint8_t i=4; i<=22; i++)
	{
		writeToBK4802(IC_ADDR, i, HIGHBYTE(rx_reg[i-4]), LOWBYTE(rx_reg[i-4]));
	}
}

void DBH_SetRxFreq(uint8_t fracn_h, uint8_t fracn_l, uint8_t div)
{
	writeToBK4802(IC_ADDR, 2, HIGHBYTE(div), LOWBYTE(div));
	writeToBK4802(IC_ADDR, 1, HIGHBYTE(fracn_l), LOWBYTE(fracn_l));
	writeToBK4802(IC_ADDR, 0, HIGHBYTE(fracn_h), LOWBYTE(fracn_h));
}
