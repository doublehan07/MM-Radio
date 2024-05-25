/*
	@Copyright Han ZHANG (DoubleHan) | All rights reserved.
	@Last Modified Time May.24, 2024
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __sEXTI_H
#define __sEXTI_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
extern uint8_t power_off_state;
extern uint8_t freq_channel_state;
extern uint8_t freq_lock;

extern uint16_t power_off_cnt;
extern uint16_t ptt_cnt;

/* Exported functions ------------------------------------------------------- */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* __sEXTI_H */
