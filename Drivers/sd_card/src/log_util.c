#include "log_util.h"

static SemaphoreHandle_t log_mutex = NULL;

/**
  * @brief  Create mutex for logging utilities
  * @retval FRESULT: result status 
  */
FRESULT logInit() {
  if (log_mutex == NULL) {
    log_mutex = xSemaphoreCreateMutex();
  }
  return FR_OK;
}

/**
  * @brief  Write/append bytes into a file, creates file if does not exist
  * @param  fileName: file name as a string
  * @param  buff: buffer that contains the data to be written
  * @param  buffSize: length of the buffer in bytes
  * @param  *bytesWritten: number of bytes successfully written to disk
  * @retval FRESULT: result status of write operation to disk 
  */
FRESULT logWrite(char* fileName, void* buff, UINT buffSize, UINT* bytesWritten) {
  FATFS fs;
  FIL fil;
  FRESULT res;
  if (xSemaphoreTake(log_mutex, (TickType_t) portMAX_DELAY) == pdPASS) {
    if (res = f_mount(&fs, "", 0) != FR_OK) {
      xSemaphoreGive(log_mutex);
      return res;
    }
    
    if (res = f_open(&fil, fileName, FA_WRITE | FA_OPEN_APPEND) != FR_OK) {
      xSemaphoreGive(log_mutex);
      return res;
    }
    
    if (res = f_write(&fil, buff, buffSize, bytesWritten) != FR_OK) {
      xSemaphoreGive(log_mutex);
      return res;
    }
    
    if (res = f_close(&fil) != FR_OK) {
      xSemaphoreGive(log_mutex);
      return res;
    }
    
    if (res = f_mount(0, "", 0) != FR_OK) {
      xSemaphoreGive(log_mutex);
      return res;
    }
    xSemaphoreGive(log_mutex);
  }
  return FR_OK;
}

/**
  * @brief  Read bytes from file, starting from file beginning
  * @param  fileName: name of the file to be read from, error if the file does not exist
  * @param  buff: buffer that will receive the data read from disk
  * @param  buffSize: length of the buffer in bytes
  * @param  *bytesRead: number of bytes successfully read from disk
  * @retval FRESULT: result status of read operation to disk
  */
FRESULT logRead(char* fileName, void* buff, UINT buffSize, UINT* bytesRead) {
  FATFS fs;
  FIL fil;
  FRESULT res;
  if (xSemaphoreTake(log_mutex, (TickType_t) portMAX_DELAY) == pdPASS) {
    if (res = f_mount(&fs, "", 0) != FR_OK) {
      xSemaphoreGive(log_mutex);
      return res;
    }
    
    if (res = f_open(&fil, fileName, FA_READ) != FR_OK) {
      xSemaphoreGive(log_mutex);
      return res;
    }
    
    if (res = f_read(&fil, buff, buffSize, bytesRead) != FR_OK) {
      xSemaphoreGive(log_mutex);
      return res;
    }
    
    if (res = f_close(&fil) != FR_OK) {
      xSemaphoreGive(log_mutex);
      return res;
    }

    if (res = f_mount(0, "", 0) != FR_OK) {
      xSemaphoreGive(log_mutex);
      return res;
    }
    xSemaphoreGive(log_mutex);
  }
  return FR_OK;
}
