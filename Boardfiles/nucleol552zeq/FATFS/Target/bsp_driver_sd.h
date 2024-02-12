#ifndef __STM32L5_SD_H
#define __STM32L5_SD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l5xx_hal.h"
#include "diskio.h"

/* Exported types ------------------------------------------------------------*/
/**
  * @brief SD Card information structure
  */
typedef HAL_SD_CardInfoTypeDef BSP_SD_CardInfo;

/* Exported constants --------------------------------------------------------*/
/**
  * @brief  SD status structure definition
  */
#define   MSD_OK                        ((BYTE)0x00)
#define   MSD_ERROR                     ((BYTE)0x01)
#define   MSD_ERROR_SD_NOT_PRESENT      ((BYTE)0x02)

/**
  * @brief  SD transfer state definition
  */
#define   SD_TRANSFER_OK                ((BYTE)0x00)
#define   SD_TRANSFER_BUSY              ((BYTE)0x01)
#define   SD_TRANSFER_ERROR             ((BYTE)0x02)

#define SD_DATATIMEOUT           ((UINT)100000000)

#define SD_PRESENT               ((BYTE)0x01)
#define SD_NOT_PRESENT           ((BYTE)0x00)

#define SD_DETECT_PIN         GPIO_PIN_9
#define SD_DETECT_GPIO_PORT   GPIOB

/* Exported functions --------------------------------------------------------*/
BYTE BSP_SD_Init(void);
BYTE BSP_SD_GetCardState(void);
BYTE BSP_SD_IsDetected(void);
DSTATUS SD_CheckStatus(BYTE lun);
BYTE BSP_PlatformIsDetected(void);

#ifdef __cplusplus
}
#endif
#endif /* __STM32L5_SD_H */