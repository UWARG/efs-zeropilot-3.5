/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sd_diskio.c (for L5)
  * @brief   SD Disk I/O driver
  * @note    To be completed by the user according to the project board in use.
  *          (see templates available in the FW pack, Middlewares\Third_Party\FatFs\src\drivers folder).
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
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define QUEUE_SIZE         (uint32_t) 10
#define READ_CPLT_MSG      (uint32_t) 1
#define WRITE_CPLT_MSG     (uint32_t) 2
#define SD_TIMEOUT 30 * 1000
#define SD_DEFAULT_BLOCK_SIZE 512
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
osMessageQueueId_t SDQueueID = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
DSTATUS SD_initialize (BYTE);
DSTATUS SD_status (BYTE);
DRESULT SD_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
  DRESULT SD_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT SD_ioctl (BYTE, BYTE, void*);
#endif  /* _USE_IOCTL == 1 */

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,
  SD_status,
  SD_read,
#if  _USE_WRITE == 1
  SD_write,
#endif /* _USE_WRITE == 1 */

#if  _USE_IOCTL == 1
  SD_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/**
  * @brief  Initializes a Drive
  * @param  pdrv : Physical drive number
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_initialize(BYTE pdrv)
{
  /* USER CODE BEGIN SD_initialize */
  Stat = STA_NOINIT;

  /* Place for user code (may require BSP functions/defines to be added to the project) */
  if (osKernelGetState() == osKernelRunning) {
    if (BSP_SD_Init() == MSD_OK) {
			Stat = SD_CheckStatus(pdrv);
		}

		if (Stat != STA_NOINIT) {
			if (SDQueueID == NULL) {
        SDQueueID = osMessageQueueNew(QUEUE_SIZE, 2, NULL);
      }
      
      if (SDQueueID == NULL) {
				Stat |= STA_NOINIT;
			}
    }
  }

  return Stat;
  /* USER CODE END SD_initialize */
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv : Physical drive number
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_status(BYTE pdrv)
{
  /* USER CODE BEGIN SD_status */
	return SD_CheckStatus(pdrv);
  /* USER CODE END SD_status */
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT SD_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
  /* USER CODE BEGIN SD_read */
  DRESULT res = RES_ERROR;

  /* Place for user code (may require BSP functions/defines to be added to the project) */

  return res;
  /* USER CODE END SD_read */
}

/**
  * @brief  Writes Sector(s)
  * @param  lun : not used
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT SD_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  /* USER CODE BEGIN SD_write */
  DRESULT res = RES_ERROR;

  /* Place for user code (may require BSP functions/defines to be added to the project) */

  return res;

  /* USER CODE END SD_write */
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT SD_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  /* USER CODE BEGIN SD_ioctl */
  DRESULT res = RES_ERROR;

  /* Place for user code (may require BSP functions/defines to be added to the project) */
  return res;

  /* USER CODE END SD_ioctl */
}
#endif /* _USE_IOCTL == 1 */

/* USER CODE BEGIN UserCode */
/* can be used to add code */
/* USER CODE END UserCode */
