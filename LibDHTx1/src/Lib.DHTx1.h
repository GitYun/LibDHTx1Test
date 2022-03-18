/**
 * \file Lib.DHTx1.h
 * \author vEmagic (admin@vemgic.com)
 * \brief
 * \version 0.1.0
 * \date 2020-10-29
 *
 * Copyright 2020 (C), Sichuan Deyang DeYi Electronic Instrument Co., Ltd.
 *
 */

#ifndef __LIB_DHTx1_H
#define __LIB_DHTx1_H

// #include "main.h"

#if defined STM32
#include "stm32f0xx_hal.h"

typedef GPIO_TypeDef tPort;
typedef uint32_t tPin;

#define DHTx1_DelayUs(us)
#define DHTx1_Millis() 0

#define DHTx1_DATA_PIN_OUTPUT_MODE(port, pin) \
    LL_GPIO_SetPinMode((port), (pin), LL_GPIO_MODE_OUTPUT)

#define DHTx1_DATA_PIN_INPUT_MODE(port, pin) \
    LL_GPIO_SetPinMode((port), (pin), LL_GPIO_MODE_INPUT)

#define DHTx1_DATA_PIN_HIGH(port, pin) LL_GPIO_SetOutputPin((port), (pin))
#define DHTx1_DATA_PIN_LOW(port, pin) LL_GPIO_ResetOutputPin((port), (pin))

#define DHTx1_DATA_PIN_READ(port, pin) LL_GPIO_IsInputPinSet((port), (pin))

#elif defined (SDCC) || defined (__SDCC)
#include <stc12.h>
#include "../../src/systimer.h"

#define DHTx1_DATA_PIN P1_7

typedef uint8_t tPort;
typedef uint8_t tPin;

#define DHTx1_DelayUs(us) \
do {                      \
    Timer0Restart(us);    \
    while (false == Timer0IsElapsed()); \
} while (0)

#define DHTx1_Millis() 0

#define DHTx1_DATA_PIN_OUTPUT_MODE(port, pin)
#define DHTx1_DATA_PIN_INPUT_MODE(port, pin)

#define DHTx1_DATA_PIN_HIGH(port, pin) (DHTx1_DATA_PIN = 1)
#define DHTx1_DATA_PIN_LOW(port, pin) (DHTx1_DATA_PIN = 0)

#define DHTx1_DATA_PIN_READ(port, pin) (DHTx1_DATA_PIN)

#else
#error "Unkown MCU platform!"
#endif
/*-----------------------------------------------------------------------------------------------*/

typedef enum {
    eDHTx1_INVAILD = -1,
    eDHTx1_OPENED = 0,
    eDHTx1_CLOSED,
    eDHTx1_DATA_CHECK_SUM_OK,
    eDHTx1_OPEN_RSP_LOW_ERROR,
    eDHTx1_OPEN_RSP_HIGH_ERROR,
    eDHTx1_DATA_CHECK_SUM_ERROR,
} tDHTx1Status;

typedef enum {
    eDHT11 = 0,
    eDHT21,
} tDHTx1Id;

typedef struct {
    tPort sPort;
    tPin sPin;
} tDHTx1Gpio;

typedef struct {
    uint8_t pui8Data[5];
    tDHTx1Id sId;
    tDHTx1Gpio* psGPIO;
    tDHTx1Status sStatus;
    uint32_t ui32LastReadTime;
} tDHTx1Sensor;
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn int8_t DHTx1_Init(tDHTx1Sensor* psDHTx1)
 *
 * \brief Initial DHTx1 Device
 *
 * \param psDHTx1 Pointer to the \ref tDHTx1Sensor instance
 * \return int8_t 0 - Success, or Failed
 */
int8_t DHTx1_Init(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn tDHTx1Status DHTx1_Open(tDHTx1Sensor* psDHTx1)
 *
 * \brief Open the DHTx1 Device
 *
 * \param psDHTx1 Pointer to the \ref tDHTx1Sensor instance
 * \return tDHTx1Status If open success, return the enum 'eDHTx1_OPENED';
 *                      or if DHTx1 response low level error, return 'eDHTx1_OPEN_RSP_LOW_ERROR',
 *                      if DHTx1 response high level error, return 'eDHTx1_OPEN_RSP_HIGH_ERROR'
 */
tDHTx1Status DHTx1_Open(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn bool DHTx1_Close(tDHTx1Sensor* psDHTx1)
 * \brief Colse the DHTx1 Device
 *
 * \param psDHTx1 Pointer to the \ref tDHTx1Sensor instance
 * \return true Close Sucess, or false
 */
bool DHTx1_Close(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn uint8_t DHTx1_ReadByte(tDHTx1Sensor* psDHTx1)
 * \brief Read a byte data from the DHTx1 Device
 *
 * \param psDHTx1 Pointer to the \ref tDHTx1Sensor instance
 * \return uint8_t The DHTx1 device data
 */
uint8_t DHTx1_ReadByte(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn bool DHTx1_ReadData(tDHTx1Sensor* psDHTx1)
 * \brief Read the DHTx1 Device Data
 *
 * \param psDHTx1 Pointer the \ref tDHTx1Sensor instance
 * \return true
 * \return false
 */
bool DHTx1_ReadData(tDHTx1Sensor* psDHTx1);

/**
 * \fn bool DHTx1_IsShortOfReadTime(void)
 * \brief Check that the interval between reading data is too short
 *
 * \return true Interval between reading data is too short, or false
 */
bool DHTx1_IsShortOfReadTime(tDHTx1Sensor* psDHTx1, uint32_t ui32Intervals);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn bool DHTx1_IsDataChanged(tDHTx1Sensor* psDHTx1)
 * \brief Check the data whether has been changed
 *
 * \param psDHTx1 Pointer the \ref tDHTx1Sensor instance
 * \param pui8PreviousData Pointer the previous data buffer
 * \return true The data has been changed, or false
 */
bool DHTx1_IsDataChanged(tDHTx1Sensor* psDHTx1, uint8_t* pui8PreviousData);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn tTemperature DHTx1_GetTemperature(tDHTx1Sensor* psDHTx1)
 * \brief Get the temperature value
 *
 * \param psDHTx1 Pointer to the \ref tDHTx1Sensor instance
 * \return uin16_t Temperature value scale 10 time
 */
uint16_t DHTx1_GetTemperature(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn tHumidity DHTx1_GetHumidity(tDHTx1Sensor* psDHTx1)
 * \brief Get the humidity value
 *
 * \param psDHTx1 Pointer to the \ref tDHTx1Sensor instance
 * \return uint16_t Humidity value scale 10 time
 */
uint16_t DHTx1_GetHumidity(tDHTx1Sensor* psDHTx1);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn DHTx1_GetHumidityAndTemperature(tDHTx1Sensor* psDHTx1, uint16_t* pui16Humidity, uint16_t* pui16Temperature)
 * \brief
 *
 * \param psDHTx1
 * \param pui16Humidity
 * \param pui16Temperature
 */
void DHTx1_GetHumidityAndTemperature(tDHTx1Sensor* psDHTx1,
    uint16_t* pui16Humidity, uint16_t* pui16Temperature);

/**
 * \fn float DHTx1_ConvertC2F(uint16_t ui16Celsius)
 * \brief Convert the temperature from celsius to fahrenheit
 *
 * \param ui16Celsius The celsius temperature value, the value scale 10 time
 * \return float Fahrenheit temperature
 */
float DHTx1_ConvertC2F(uint16_t ui16Celsius);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn float DHTx1_ConvertC2K(uint16_t ui16Celsius)
 * \brief Convert the temperature from celsius to kai
 *
 * \param ui16Celsius The celsius temperature value, the value scale 10 time
 * \return float Kai temperature
 */
float DHTx1_ConvertC2K(uint16_t ui16Celsius);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn float DHTx1_DewPoint(uint16_t ui16Celsius, uint16_t ui16Humidity)
 * \brief Get dew pointer value
 *
 * \param ui16Celsius The celsius temperature value, the value scale 10 time
 * \param ui16Humidity The humidity value, the value scale 10 time
 * \return float Dew Pointer value
 */
float DHTx1_DewPoint(uint16_t ui16Celsius, uint16_t ui16Humidity);
/*-----------------------------------------------------------------------------------------------*/

/**
 * \fn float DHTx1_DewPointFast(uint16_t ui16Celsius, uint16_t ui16Humidity)
 * \brief Get dew pointer value
 *
 * \param ui16Celsius The celsius temperature value, the value scale 10 time
 * \param ui16Humidity The humidity value, the value scale 10 time
 * \return float Dew Pointer value
 */
float DHTx1_DewPointFast(uint16_t ui16Celsius, uint16_t ui16Humidity);
/*-----------------------------------------------------------------------------------------------*/

#endif

/*************************************************************************************************
************* Copyright 2020 (C), Sichuan Deyang DeYi Electronic Instrument Co., Ltd *************
*************************************************************************************************/
