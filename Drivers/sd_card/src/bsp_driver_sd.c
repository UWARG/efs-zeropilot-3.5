/* Includes ------------------------------------------------------------------*/
#include "bsp_driver_sd.h"
#include "diskio.h"
#include "cmsis_os.h"

/* Extern variables ---------------------------------------------------------*/
extern SD_HandleTypeDef hsd1;
extern osMessageQueueId_t SDQueueID;

DSTATUS SD_CheckStatus(BYTE lun)
{
	DSTATUS Stat = STA_NOINIT;

  if(BSP_SD_GetCardState() == SD_TRANSFER_OK)
  {
    Stat &= ~STA_NOINIT;
  }

  return Stat;
}

BYTE BSP_SD_Init(void)
{
  BYTE sdState = MSD_OK;
  /* Check if the SD card is plugged in the slot */
  if (BSP_SD_IsDetected() != SD_PRESENT)
  {
    return MSD_ERROR_SD_NOT_PRESENT;
  }
  /* HAL SD initialization */
  sdState = HAL_SD_Init(&hsd1);

  return sdState;
}

/**
  * @brief  Gets the current SD card data status.
  * @param  None
  * @retval Data transfer state.
  *          This value can be one of the following values:
  *            @arg  SD_TRANSFER_OK: No data transfer is acting
  *            @arg  SD_TRANSFER_BUSY: Data transfer is acting
  */
BYTE BSP_SD_GetCardState(void)
{
  return ((HAL_SD_GetCardState(&hsd1) == HAL_SD_CARD_TRANSFER ) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @param  None
 * @retval Returns if SD is detected or not
 */
BYTE BSP_SD_IsDetected(void)
{
  __IO BYTE status = SD_PRESENT;

  if (BSP_PlatformIsDetected() == 0x0)
  {
    status = SD_NOT_PRESENT;
  }

  return status;
}

BYTE BSP_PlatformIsDetected(void) {
    BYTE status = SD_PRESENT;
    /* Check SD card detect pin */
    if(HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) != GPIO_PIN_RESET)
    {
        status = SD_NOT_PRESENT;
    }
    return status;
}

/**
  * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  ReadAddr: Address from where data is to be read
  * @param  NumOfBlocks: Number of SD blocks to read
  * @retval SD status
  */
BYTE BSP_SD_ReadBlocks_DMA(DWORD *pData, DWORD ReadAddr, DWORD NumOfBlocks) {
  BYTE sdState = MSD_OK;

  // Read block(s) in DMA transfer mode
  if (HAL_SD_ReadBlocks_DMA(&hsd1, (uint8_t *)pData, ReadAddr, NumOfBlocks) != HAL_OK) {
    sdState = MSD_ERROR;
  }

  return sdState;
}

/**
  * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
  * @param  pData: Pointer to the buffer that will contain the data to transmit
  * @param  WriteAddr: Address from where data is to be written
  * @param  NumOfBlocks: Number of SD blocks to write
  * @retval SD status
  */
BYTE BSP_SD_WriteBlocks_DMA(DWORD *pData, DWORD WriteAddr, DWORD NumOfBlocks) {
  BYTE sdState = MSD_OK;

  // Write block(s) in DMA transfer mode
  if (HAL_SD_WriteBlocks_DMA(&hsd1, (uint8_t *)pData, WriteAddr, NumOfBlocks) != HAL_OK) {
    sdState = MSD_ERROR;
  }

  return sdState;
}

int SD_CheckStatusWithTimeout(DWORD timeout) {
  DWORD timer = osKernelGetTickCount();
  while (osKernelGetTickCount() - timer < timeout) {
    if (BSP_SD_GetCardState() == SD_TRANSFER_OK) {
      return 0;
    }
  }
  return -1;
}

/**
  * @brief  Get SD information about specific SD card.
  * @param  CardInfo: Pointer to HAL_SD_CardInfoTypedef structure
  * @retval None
  */
void BSP_SD_GetCardInfo(BSP_SD_CardInfo *CardInfo) {
  // Get SD card Information
  HAL_SD_GetCardInfo(&hsd1, CardInfo);
}

/**
  * @brief Tx Transfer completed callback
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
  const uint16_t msg = 2;
  osMessageQueuePut(SDQueueID, (const void *)&msg, NULL, 0);
}

/**
  * @brief Rx Transfer completed callback
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
  const uint16_t msg = 1;
  osMessageQueuePut(SDQueueID, (const void *)&msg, NULL, 0);
}

/**
  * @brief Transfer error callback
  * @param hsd: SD handle
  * @retval None
  */
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
  const uint16_t msg = 3;
  osMessageQueuePut(SDQueueID, (const void *)&msg, NULL, 0);
}

/**
  * @brief RTOS-aware delay
  * @param delay: number of ticks to delay
  * @retval None
  */
void HAL_Delay(uint32_t delay)
{
  osDelay(delay);
}
