/*
 	Biblioteca do protocolo de comunicacao SPI1 para STM32
 	Versao em C

 	Feito por: setor de Software Baixo Nivel - Zenith Aerospace
 	Criado em: 18 jan 2020
*/

#include "ESP32_SPI.h"
#include <SPI.h>
#include "Arduino.h"

HAL_StatusTypeDef SPI_read_register(SPIClass * spi, uint16_t ss_pin, uint8_t reg_addr, uint8_t* pvalue) {
  HAL_StatusTypeDef status = HAL_OK;																// status of the execution (checks if any error occurs)
  reg_addr = reg_addr & 0x7f;

  spi->beginTransaction(SPISettings(SPICLK, MSBFIRST, SPI_MODE0));

  /* Changes SS pin to enable comunication */
  digitalWrite(ss_pin, LOW);

  /* Writes Register Address in SPI */
  spi->transfer(reg_addr);

  /* Reads Register Value from SPI */
  *pvalue = spi->transfer(0x00);

  /* Changes SS pin to disable comunication */
  digitalWrite(ss_pin, HIGH);

  spi->endTransaction();

  return status;
}

HAL_StatusTypeDef SPI_burst_read(SPIClass * spi, uint16_t ss_pin, uint8_t start_addr, uint8_t* pvalue, uint16_t size) {
  HAL_StatusTypeDef status = HAL_OK;																// status of the execution (checks if any error occurs)
  start_addr = start_addr & 0x7f;

  spi->beginTransaction(SPISettings(SPICLK, MSBFIRST, SPI_MODE0));

  /* Changes SS pin to enable comunication */
  digitalWrite(ss_pin, LOW);

  /* Writes Register Address in SPI */
  spi->transfer(start_addr);

  /* Reads Register Value from SPI */
  spi->transfer(pvalue, size);

  /* Changes SS pin to disable comunication */
  digitalWrite(ss_pin, HIGH);

  spi->endTransaction();

  return status;
}

HAL_StatusTypeDef SPI_write_register(SPIClass * spi, uint16_t ss_pin, uint8_t reg_addr, uint8_t value) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t sent_data[2];
  sent_data[0] = reg_addr | 0x80;
  sent_data[1] = value;

  spi->beginTransaction(SPISettings(SPICLK, MSBFIRST, SPI_MODE0));

  /* Changes SS pin to enable comunication */
  digitalWrite(ss_pin, LOW);

  /* Writes in Register via SPI */
  spi->transfer(sent_data, 2);

  /* Changes SS pin to disable comunication */
  digitalWrite(ss_pin, HIGH);

  spi->endTransaction();


  return status;
}

HAL_StatusTypeDef SPI_burst_write(SPIClass * spi, uint16_t ss_pin, uint8_t start_addr, uint8_t* values, uint16_t size) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t* sent_data;

  sent_data = (uint8_t*)malloc(size + 1);
  sent_data[0] = start_addr | 0x80;
  for (int i = 0; i < size; i++) {
    sent_data[i + 1] = values[i];
  }

  spi->beginTransaction(SPISettings(SPICLK, MSBFIRST, SPI_MODE0));

  /* Changes SS pin to enable comunication */
  digitalWrite(ss_pin, LOW);

  /* Writes in Register via SPI */
  spi->transfer(sent_data, size + 1);

  free(sent_data);

  /* Changes SS pin to disable comunication */
  digitalWrite(ss_pin, HIGH);

  spi->endTransaction();

  return status;
}
