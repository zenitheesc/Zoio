/*
 *  This library was stolen from Zenith Aerospace :)
 *  STM32 SPI interace library.
 *
 *  Made by:Low Level Software Department- Zenith Aerospace
 *  Created on: 18 jan 2020
 */

#ifndef ESP32_SPI_H
#define ESP32_SPI_H
#ifdef __cplusplus

extern "C" {
#endif

/* Platform Specific Includes */
#include "esp32.h"

// One might want to change to a static global variable: static const int spiClk = 1000000; // 1 MHz
#ifndef SPICLK  
#define SPICLK 2000000
#endif

/* SPI Functions */
HAL_StatusTypeDef SPI_read_register(SPIClass * spi, uint16_t ss_pin, uint8_t reg_addr, uint8_t* pvalue);
HAL_StatusTypeDef SPI_burst_read(SPIClass * spi, uint16_t ss_pin, uint8_t start_addr, uint8_t* pvalue, uint16_t size);
HAL_StatusTypeDef SPI_write_register(SPIClass * spi, uint16_t ss_pin, uint8_t reg_addr, uint8_t value);
HAL_StatusTypeDef SPI_burst_write(SPIClass * spi, uint16_t ss_pin, uint8_t start_addr, uint8_t* values, uint16_t size);
#ifdef __cplusplus
}
#endif
#endif//STM32_SPI_H
