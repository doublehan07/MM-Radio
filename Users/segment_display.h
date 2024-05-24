/*
	@Copyright Han ZHANG (DoubleHan) | All rights reserved.
	@Last Modified Time May.24, 2024
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sSEG_DISP_H
#define __sSEG_DISP_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DBH_SetNum(uint16_t numH, uint16_t numL);
void DBH_DisplayNumH(uint8_t state);
void DBH_DisplayNumL(uint8_t state);

#endif /* __sSEG_DISP_H */
