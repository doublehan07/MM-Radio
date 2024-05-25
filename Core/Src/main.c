/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "iwdg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "bk4802.h"
#include "segment_display.h"
#include "exti.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); //Turn on the LED to indicate that the script is active.
	HAL_GPIO_WritePin(PA0_GPIO_Port, PA0_Pin, GPIO_PIN_SET); //Enable CE, turn on LED1.
	DBH_DelayMS(1000);
	
	DBH_WriteTo4802(IC_ADDR, 23, 0x60, 0xFF);
	//reg_addr=23
	//B15: 0-Disable Power Saving
	//B14-B13: 11(3)-RX Active Period in Power Saving Mode=192ms
	//B12-B11: 00(0)-Sleep Period in Power Saving Mode=384ms
	//B08: 0-PIN7 is CALL, PIN24 is RSSI
	//B07-B00: 0xFF(255)-Ex-noise Threshold for Speaker ON Condition
	//0x00=speaker always off (during test)
	
	DBH_SetRXMode();
	
	DBH_WriteTo4802(IC_ADDR, 32, 0x31, 0xFF);
	//reg_addr=32
	//B14: 0-ASK Data Decision by Averaged Value
	//B13: 1-Average Speed=1024 samples
	//B12: 1-Bypass Low Pass Filter
	//B08: 1-RX Volume set by REG32<07:00>
	//B07-B00: 0xFF(255)-Set the Volume of RX Audio
	
	DBH_SetRXFreq(0x5259, 0xC651);
	DBH_SetNum(437, 625);	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//Blink the LED.
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		DBH_DelayMS(100);
		
		if (freq_lock!=0 && ptt_cnt==0) //When PTT is pressed, switch the RX frequency.
		{
			switch(freq_channel_state)
			{
				//BY1CP
				case 0: 
					DBH_SetRXFreq(0x52A8, 0x149F);
					DBH_SetNum(439, 250);
					break;
				
				//Satellite TEVEL series, considering the Doppler effect.
				case 1:
					DBH_SetRXFreq(0x521F, 0x39E5);
					DBH_SetNum(436, 410);
					break;
				case 2:
					DBH_SetRXFreq(0x521E, 0xFC27);
					DBH_SetNum(436, 405);
					break;
				case 3:
					DBH_SetRXFreq(0x521E, 0xBE69);
					DBH_SetNum(436, 400);
					break;
				case 4:
					DBH_SetRXFreq(0x521E, 0x80AB);
					DBH_SetNum(436, 395);
					break;
				case 5:
					DBH_SetRXFreq(0x521E, 0x434E);
					DBH_SetNum(436, 390);
					break;
				
				//Satellite ISS, considering the Doppler effect.
				case 6:
					DBH_SetRXFreq(0x5262, 0xB07B);
					DBH_SetNum(437, 810);
					break;
				case 7:
					DBH_SetRXFreq(0x5262, 0x72BE);
					DBH_SetNum(437, 805);
					break;
				case 8:
					DBH_SetRXFreq(0x5262, 0x3500);
					DBH_SetNum(437, 800);
					break;
				case 9:
					DBH_SetRXFreq(0x5261, 0xF7A2);
					DBH_SetNum(437, 795);
					break;
				case 10:
					DBH_SetRXFreq(0x5261, 0xB9E5);
					DBH_SetNum(437, 790);
					break;
				
				//Satellite RS40s. Happy Children's Day!
				case 11:
					DBH_SetRXFreq(0x5259, 0xC651);
					DBH_SetNum(437, 625);
					break;
				default: break;
			}

			freq_lock = 0; //Release the freq_lock to indicate that the frequency has changed.
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
