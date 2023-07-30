/*
 *  This library was stolen from Zenith Aerospace :)
 *  Generic interace library.
 *
 *  Made by:Low Level Software Department - Zenith Aerospace
 *  Created on: 20 sept 2019
 */
 
#ifndef ESP32_H
#define ESP32_H

#include <stdlib.h>
#include <stdint.h>
#include <Arduino.h>

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;


#define RETURN_ON_ERROR(status);\
	if(status != HAL_OK){\
		return status;\
	}

#endif
