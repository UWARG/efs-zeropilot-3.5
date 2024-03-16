/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l5xx_hal.h"
#include "stm32l5xx_ll_ucpd.h"
#include "stm32l5xx_ll_bus.h"
#include "stm32l5xx_ll_cortex.h"
#include "stm32l5xx_ll_rcc.h"
#include "stm32l5xx_ll_system.h"
#include "stm32l5xx_ll_utils.h"
#include "stm32l5xx_ll_pwr.h"
#include "stm32l5xx_ll_gpio.h"
#include "stm32l5xx_ll_dma.h"

#include "stm32l5xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_BL_PWM_Pin GPIO_PIN_1
#define LCD_BL_PWM_GPIO_Port GPIOE
#define LED_GREEN_Pin GPIO_PIN_12
#define LED_GREEN_GPIO_Port GPIOG
#define USER_BUTTON_Pin GPIO_PIN_13
#define USER_BUTTON_GPIO_Port GPIOC
#define LED_RED_Pin GPIO_PIN_3
#define LED_RED_GPIO_Port GPIOD
#define GYRO_ACC_INT_Pin GPIO_PIN_3
#define GYRO_ACC_INT_GPIO_Port GPIOF
#define UCPD_DBn_Pin GPIO_PIN_5
#define UCPD_DBn_GPIO_Port GPIOB
#define USART1_TX_Pin GPIO_PIN_9
#define USART1_TX_GPIO_Port GPIOA
#define USART1_RX_Pin GPIO_PIN_10
#define USART1_RX_GPIO_Port GPIOA
#define LCD_TE_Pin GPIO_PIN_8
#define LCD_TE_GPIO_Port GPIOA
#define SDIO_DETECT_Pin GPIO_PIN_2
#define SDIO_DETECT_GPIO_Port GPIOF
#define CTP_INT_Pin GPIO_PIN_1
#define CTP_INT_GPIO_Port GPIOF
#define LCD_PWR_ON_Pin GPIO_PIN_0
#define LCD_PWR_ON_GPIO_Port GPIOH
#define BLE_INT_Pin GPIO_PIN_6
#define BLE_INT_GPIO_Port GPIOG
#define BLE_RSTN_Pin GPIO_PIN_8
#define BLE_RSTN_GPIO_Port GPIOG
#define BLE_CSN_Pin GPIO_PIN_5
#define BLE_CSN_GPIO_Port GPIOG
#define LCD_RST_Pin GPIO_PIN_14
#define LCD_RST_GPIO_Port GPIOF
#define STMOD_SEL_12_Pin GPIO_PIN_11
#define STMOD_SEL_12_GPIO_Port GPIOF
#define AUDIO_RESETN_Pin GPIO_PIN_1
#define AUDIO_RESETN_GPIO_Port GPIOG
#define UCPD_FLT_Pin GPIO_PIN_14
#define UCPD_FLT_GPIO_Port GPIOB
#define VBUS_SENSE_Pin GPIO_PIN_4
#define VBUS_SENSE_GPIO_Port GPIOA
#define STMOD_SEL_34_Pin GPIO_PIN_12
#define STMOD_SEL_34_GPIO_Port GPIOF
#define CTP_RST_Pin GPIO_PIN_15
#define CTP_RST_GPIO_Port GPIOF

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
