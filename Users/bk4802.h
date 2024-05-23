/*
	@Copyright Han ZHANG (DoubleHan) | All rights reserved.
	@BK4802 communication protocol credit to BG7QKU
	@For more details, please visit https://github.com/BG7QKU
	@Last Modified Time May.24, 2024
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sBK4802_H
#define __sBK4802_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define HIGHBYTE(x) ((x) >> 8)
#define LOWBYTE(x)	((x) & 0xFF)

extern unsigned short rx_reg[];
extern unsigned short tx_reg[];

/* Exported functions ------------------------------------------------------- */
void DBH_BK4802_Init(void);
void DBH_SetToRxMode(void);
void DBH_SetRxFreq(uint8_t fracn_h, uint8_t fracn_l, uint8_t div);

void writing(unsigned char ICaddforwrite,unsigned char hadd,unsigned char hdata,unsigned char ldata);

#endif /* __sBK4802_H */
