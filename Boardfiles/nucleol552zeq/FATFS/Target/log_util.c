#include "log_util.h"

/**
  * @brief  Write/append bytes into a file, creates file if does not exist
  * @param  file_name: file name as a string
  * @param  buff: buffer that contains the data to be written
  * @param  buff_sz: length of the buffer in bytes
  * @param  *bytes_written: number of bytes successfully written to disk
  * @retval FRESULT: result status of write operation to disk 
  */
FRESULT logWrite(char* file_name, void* buff, UINT buff_sz, UINT* bytes_written) {
  FATFS fs;
  FIL fil;
  FRESULT res;

  if (res = f_mount(&fs, "", 0) != FR_OK)
    return res;
  
  if (res = f_open(&fil, file_name, FA_WRITE | FA_OPEN_APPEND) != FR_OK)
    return res;
  
  if (res = f_write(&fil, buff, buff_sz, bytes_written) != FR_OK)
    return res;
  
  if (res = f_close(&fil) != FR_OK)
    return res;
  
  if (res = f_mount(0, "", 0) != FR_OK)
    return res;

  return FR_OK;
}

/**
  * @brief  Read bytes from file, starting from file beginning
  * @param  file_name: name of the file to be read from, error if the file does not exist
  * @param  buff: buffer that will receive the data read from disk
  * @param  buff_sz: length of the buffer in bytes
  * @param  *bytes_read: number of bytes successfully read from disk
  * @retval FRESULT: result status of read operation to disk
  */
FRESULT logRead(char* file_name, void* buff, UINT buff_sz, UINT* bytes_read) {
  FATFS fs;
  FIL fil;
  FRESULT res;

  if (res = f_mount(&fs, "", 0) != FR_OK)
    return res;
  
  if (res = f_open(&fil, file_name, FA_READ) != FR_OK)
    return res;
  
  if (res = f_read(&fil, buff, buff_sz, bytes_read) != FR_OK)
    return res;
  
  if (res = f_close(&fil) != FR_OK)
    return res;

  if (res = f_mount(0, "", 0) != FR_OK)
    return res;

  return FR_OK;
}

char* _print_log_err(FRESULT r) {
  switch (r) {
    case FR_OK:
        return "FR_OK\r\n";
    case FR_DISK_ERR:
        return "FR_DISK_ERR\r\n";
    case FR_INT_ERR:
        return "FR_INT_ERR\r\n";
    case FR_NOT_READY:
        return "FR_NOT_READY\r\n";
    case FR_NO_FILE:
        return "FR_NO_FILE\r\n";
    case FR_NO_PATH:
        return "FR_NO_PATH\r\n";
    case FR_INVALID_NAME:
        return "FR_INVALID_NAME\r\n";
    case FR_DENIED:
        return "FR_DENIED\r\n";
    case FR_EXIST:
        return "FR_EXIST\r\n";
    case FR_INVALID_OBJECT:
        return "FR_INVALID_OBJECT\r\n";
    case FR_WRITE_PROTECTED:
        return "FR_WRITE_PROTECTED\r\n";
    case FR_INVALID_DRIVE:
        return "FR_INVALID_DRIVE\r\n";
    case FR_NOT_ENABLED:
        return "FR_NOT_ENABLED\r\n";
    case FR_NO_FILESYSTEM:
        return "FR_NO_FILESYSTEM\r\n";
    case FR_MKFS_ABORTED:
        return "FR_MKFS_ABORTED\r\n";
    case FR_TIMEOUT:
        return "FR_TIMEOUT\r\n";
    case FR_LOCKED:
        return "FR_LOCKED\r\n";
    case FR_NOT_ENOUGH_CORE:
        return "FR_NOT_ENOUGH_CORE\r\n";
    case FR_TOO_MANY_OPEN_FILES:
        return "FR_TOO_MANY_OPEN_FILES\r\n";
    case FR_INVALID_PARAMETER:
        return "FR_INVALID_PARAMETER\r\n";
    default:
        return "Unrecognized error\r\n";
  }
}
