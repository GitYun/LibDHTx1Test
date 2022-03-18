#include <stc12.h>
#include "ser_ir.h"
#include "itoa.h"
#include "../LibDHTx1/src/Lib.DHTx1.h"
#include "systimer.h"

#define LED P0_0
#define SYSTEM_CLOCK 11095200UL

#define LIMIT_OUTPUT_NUMBER(num) \
do { \
    char str[4] = {0}; \
    static uint16_t ui16Minutes = 0; \
    ++ui16Minutes; \
    itoa(ui16Minutes, str, 10); \
    if (ui16Minutes > (num)) { \
        ser_puts("Timerout: "); \
        ser_puts(str); \
        for(;;); \
    } \
    ser_puts(str); \
    ser_putc(':'); \
} while (0)
/*-----------------------------------------------------------------------------------------------*/

tDHTx1Sensor g_sDHT11;

void AppDHT11Init(void)
{
    static tDHTx1Gpio sGPio = {.sPort = 1, .sPin = 7};

    g_sDHT11.psGPIO = &sGPio;
    g_sDHT11.sId = eDHT11;
    DHTx1_Init(&g_sDHT11);
}
/*-----------------------------------------------------------------------------------------------*/

void main(void)
{
    uint16_t ui16Temperature = 0;
    uint16_t ui16Humidity = 0;

    EA = 1;
    LED = 1;

    AppDHT11Init();
    Timer0Init();
    ser_init();

    for (;;)
    {
        char ch = 0;        
        char strHumidity[4] = {0};
        char strTemperatrue[4] = {0};

        DelayMs(500);
        LIMIT_OUTPUT_NUMBER(120);

        if (true == DHTx1_ReadData(&g_sDHT11))
        {
            LED = !LED;

            //    ui16Humidity = DHTx1_GetHumidity(&g_sDHT11);
            //    ui16Temperature = DHTx1_GetTemperature(&g_sDHT11);
            DHTx1_GetHumidityAndTemperature(&g_sDHT11, &ui16Humidity, &ui16Temperature);

            uitoa(ui16Humidity, strHumidity, 10);
            uitoa(ui16Temperature, strTemperatrue, 10);
            ser_puts(strHumidity);
            ser_putc(' ');
            ser_puts(strTemperatrue);
            ser_puts("\r");
        }
        else // for debug
        {
            ser_putc(g_sDHT11.pui8Data[0]);
            ser_putc(g_sDHT11.pui8Data[1]);
            ser_putc(g_sDHT11.pui8Data[2]);
            ser_putc(g_sDHT11.pui8Data[3]);
            ser_putc(g_sDHT11.pui8Data[4]);
            ser_putc('e');
        }
    }
}
/*-----------------------------------------------------------------------------------------------*/
