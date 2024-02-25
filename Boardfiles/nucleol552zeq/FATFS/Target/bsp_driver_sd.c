/* Includes ------------------------------------------------------------------*/
#include "bsp_driver_sd.h"
#include "diskio.h"
#include "cmsis_os.h"

/* Extern variables ---------------------------------------------------------*/
extern SD_HandleTypeDef hsd1;

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
  BYTE sd_state = MSD_OK;
  /* Check if the SD card is plugged in the slot */
  if (BSP_SD_IsDetected() != SD_PRESENT)
  {
    return MSD_ERROR_SD_NOT_PRESENT;
  }
  /* HAL SD initialization */
  sd_state = HAL_SD_Init(&hsd1);

  return sd_state;
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

