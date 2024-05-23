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
0x0300,
0x8e04,
0xF140,
0xED00,
0x17E0,
0xe0e0,
0x8543,
0x0700,
0xA066,
0xFFFF,
0xFFE0,
0x07a0,
0x9E3C,
0x1F00,
0xD1c1,
0x200F,
0x01FF,
0xE000,
0x0339
};

uint16_t tx_reg[]={
0x7C00,
0x0c04,
0xF140,
0xED00,
0x3fE0,
0xe0e0,
0x8543,
0x0700,
0xA066,
0xFFFF,
0xffe0,
0x061f,
0x9e3c,
0x1f00,
0xd1C1,
0x200f,
0x01FF,
0xE000,
0x0c00
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
	unsigned char i;
	for(i=0;i<8;i++)
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
	writeToBK4802(IC_ADDR, 23, 0xA8, 0xD0);
}

void DBH_SetToRxMode(void)
{
	uint8_t i = 0;
	for (i = 4; i <=22; i++)
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
