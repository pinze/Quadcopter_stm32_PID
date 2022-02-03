/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
void channelsearch(void);
void pid_caculate(void);
void init_dmp_data(void);
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LD_Pin GPIO_PIN_13
#define LD_GPIO_Port GPIOC
#define esc1_Pin GPIO_PIN_0
#define esc1_GPIO_Port GPIOA
#define esc2_Pin GPIO_PIN_1
#define esc2_GPIO_Port GPIOA
#define esc3_Pin GPIO_PIN_2
#define esc3_GPIO_Port GPIOA
#define esc4_Pin GPIO_PIN_3
#define esc4_GPIO_Port GPIOA
#define ch1_Pin GPIO_PIN_6
#define ch1_GPIO_Port GPIOA
#define ch2_Pin GPIO_PIN_7
#define ch2_GPIO_Port GPIOA
#define ch3_Pin GPIO_PIN_0
#define ch3_GPIO_Port GPIOB
#define ch4_Pin GPIO_PIN_1
#define ch4_GPIO_Port GPIOB
#define LDD_Pin GPIO_PIN_12
#define LDD_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_6
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
