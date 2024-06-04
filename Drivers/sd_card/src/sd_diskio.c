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
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define QUEUE_SIZE         (uint32_t) 10
#define READ_CPLT_MSG      (uint32_t) 1
#define WRITE_CPLT_MSG     (uint32_t) 2
#define TRANS_ERR_MSG      (uint32_t) 3
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
static DWORD alignedBuffer[BLOCKSIZE / 4] = {0};
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
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_initialize(BYTE lun)
{
  /* USER CODE BEGIN SD_initialize */
  Stat = STA_NOINIT;
  BYTE pdrv = lun; // physical drive number

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
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SD_status(BYTE lun)
{
  /* USER CODE BEGIN SD_status */
  BYTE pdrv = lun; // physical drive number

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
  WORD event;
	osStatus_t status;

  // DMA can only handle word-aligned pointers
  // If BYTE* buff is not word aligned
  if ((((uintptr_t)buff) & 0x3) != 0) {
    for (UINT i = 0; i < count; i++) {
      res = RES_ERROR;

      // Ensure the SDCard is ready for a new operation
      if (SD_CheckStatusWithTimeout(SD_TIMEOUT) < 0) {
        return res;
      }

      if (BSP_SD_ReadBlocks_DMA(alignedBuffer, (DWORD) (sector), 1) == MSD_OK) {
        status = osMessageQueueGet(SDQueueID, (void*) &event, NULL, SD_TIMEOUT);
        if ((status == osOK) && (event == READ_CPLT_MSG)) {
          if (SD_CheckStatusWithTimeout(SD_TIMEOUT) == 0) {
            res = RES_OK;
            memcpy(buff, alignedBuffer, BLOCKSIZE);
          }
        }
      }

      if (res != RES_OK) {
        return res;
      }

      buff += BLOCKSIZE;
      sector++;
    }
  } else {
    // Ensure the SDCard is ready for a new operation
    if (SD_CheckStatusWithTimeout(SD_TIMEOUT) < 0) {
      return res;
    }

    if (BSP_SD_ReadBlocks_DMA((DWORD*) buff, (DWORD) (sector), count) == MSD_OK) {
      status = osMessageQueueGet(SDQueueID, (void*) &event, NULL, SD_TIMEOUT);
      if ((status == osOK) && (event == READ_CPLT_MSG)) {
        if (SD_CheckStatusWithTimeout(SD_TIMEOUT) == 0) {
          res = RES_OK;
        }
      }
    }
  }

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
	WORD event;
	osStatus_t status;

  // DMA can only handle word-aligned pointers
  // If BYTE* buff is not word aligned
  if ((((uintptr_t)buff) & 0x3) != 0) {
    for (UINT i = 0; i < count; i++) {
      res = RES_ERROR;
      memcpy(alignedBuffer, buff, BLOCKSIZE);

      // Ensure the SDCard is ready for a new operation
      if (SD_CheckStatusWithTimeout(SD_TIMEOUT) < 0) {
        return res;
      }

      if (BSP_SD_WriteBlocks_DMA(alignedBuffer, (DWORD) (sector), 1) == MSD_OK) {
        status = osMessageQueueGet(SDQueueID, (void*) &event, NULL, SD_TIMEOUT);
        if ((status == osOK) && (event == WRITE_CPLT_MSG )) {
          if (SD_CheckStatusWithTimeout(SD_TIMEOUT) == 0) {
            res = RES_OK;
          }
        }
      }

      if (res != RES_OK) {
        return res;
      }

      buff += BLOCKSIZE;
      sector++;
    }
  } else {
    // Ensure the SDCard is ready for a new operation
    if (SD_CheckStatusWithTimeout(SD_TIMEOUT) < 0) {
      return res;
    }

    if (BSP_SD_WriteBlocks_DMA((DWORD*) buff, (DWORD) (sector), count) == MSD_OK) {
      status = osMessageQueueGet(SDQueueID, (void*) &event, NULL, SD_TIMEOUT);
      if ((status == osOK) && (event == WRITE_CPLT_MSG )) {
        if (SD_CheckStatusWithTimeout(SD_TIMEOUT) == 0) {
          res = RES_OK;
        }
      }
    }
  }

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
	BSP_SD_CardInfo CardInfo;

	if (Stat & STA_NOINIT)
		return RES_NOTRDY;

	switch (cmd) {
		// Make sure that no pending write process
		case CTRL_SYNC:
			res = RES_OK;
		  break;

		// Get number of sectors on the disk (DWORD)
		case GET_SECTOR_COUNT:
			BSP_SD_GetCardInfo(&CardInfo);
			*((DWORD*) buff) = CardInfo.LogBlockNbr;
			res = RES_OK;
		  break;

		// Get R/W sector size (WORD)
		case GET_SECTOR_SIZE:
			BSP_SD_GetCardInfo(&CardInfo);
			*((WORD*) buff) = CardInfo.LogBlockSize;
			res = RES_OK;
		  break;

		// Get erase block size in unit of sector (DWORD)
		case GET_BLOCK_SIZE:
			BSP_SD_GetCardInfo(&CardInfo);
			*((DWORD*) buff) = CardInfo.LogBlockSize / SD_DEFAULT_BLOCK_SIZE;
			res = RES_OK;
		  break;

		default:
			res = RES_PARERR;
      break;
	}

	return res;
  /* USER CODE END SD_ioctl */
}
#endif /* _USE_IOCTL == 1 */

/* USER CODE BEGIN UserCode */
/* can be used to add code */
/* USER CODE END UserCode */
