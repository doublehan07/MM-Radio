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
#define IC_ADDR			0x90

/* Exported functions ------------------------------------------------------- */
void DBH_WriteTo4802(uint8_t IC_addr, uint8_t reg_addr, uint8_t data_h, uint8_t data_l);
void DBH_SetRXMode(void);
void DBH_SetRXFreq(uint16_t fracn_h, uint16_t fracn_l);

#endif /* __sBK4802_H */
