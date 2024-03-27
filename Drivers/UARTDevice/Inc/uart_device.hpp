#ifndef UART_DEVICE_HPP
#define UART_DEVICE_HPP

#include <cstdint>
#include "usart.h"
#include "circular_buffer.hpp"

class UARTDevice {
    public:
        /* Size of the internal buffer */
        static const uint16_t BUFFER_SIZE = 280;

        /** @brief Constructor for the UARTDevice driver.
        *
        *  The driver will manage the transmission and reception of
        *  data via a specified HAL UART handle.
        *
        *  @param huart The HAL UART handle to be managed.
        */
        UARTDevice(UART_HandleTypeDef* huart);

        /** @brief Initialize the driver.
        *
        *  Start the DMA interrupt of the UART.
        *
        *  @return Void.
        */
        void init();

        /** @brief DMA interrupt callback function.
        *
        *  This is called inside a DMA ISR to write DMA data to
        *  the driver's internal buffer.
        *
        *  @param size Size of the DMA data available.
        *  @return Void.
        */
        void callback(uint16_t size);

        /** @brief Compares a UART handle with the driver's handle
        *
        *  This is called inside a DMA ISR to see if the ISR's UART
        *  handle is the same as the one managed by the driver.
        *
        *  @param huart The UART handle to be compared.
        *  @return True if the handles are the same.
        */
        bool matchUART(UART_HandleTypeDef* huart);

        /** @brief Transmit data via UART
        *
        *  @param buf The buffer that contains data to be transmitted
        *  @param size Size of the data to be transmitted
        *  @return Void.
        */
        void transmit(uint8_t* buf, uint16_t size);

        /** @brief Get the size of the available data received from the
        *          UART and stored in the internal buffer.
        *
        *  @return Size of available data to be read in bytes.
        */
        uint16_t getAvailDataSize();

        /** @brief Read data received from UART
        *
        *  Read data that has been received from the UART and stored in
        *  the driver's internal buffer.
        *
        *  @param buf Buffer to read the data into.
        *  @param size Size of the data to be read
        *  @return True if read succeeded. False if we're trying to read
        *          more than the data available.
        */
        bool read(uint8_t* buf, uint16_t size);

    private:
        static const uint16_t TRANSMIT_TIMEOUT = 1000;
        UART_HandleTypeDef* uart_handle_;
        uint8_t buf_[BUFFER_SIZE];
        CircularBuffer circular_buf_;
        uint8_t it_recv_buf_[BUFFER_SIZE];
};

#endif