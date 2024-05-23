/*
	@Copyright Han ZHANG (DoubleHan) | All rights reserved.
	@Last Modified Time Apr.29, 2023
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sDELAY_H
#define __sDELAY_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DBH_DecTick(void);
//uint32_t GetTick(void);
//void SetTick(uint32_t setTick);
void DBH_DelayMS(uint32_t delayTimeMS);
//void DBH_DelayUS(uint16_t delayTimeUS);

#endif /* __sDELAY_H */
