#include <stdio.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include "co2.h"
#include <mh_z19.h>

extern EventGroupHandle_t EventGroupHandle;
#define BIT_1 (1 << 1)
// try
static mh_z19_returnCode_t rc;
static uint16_t co2 = 0;
void co2_sensorInit(void);
void co2CallBack(uint16_t ppmCall);
void runCo2(void);
void co2Task(void *p);
uint16_t getCo2(void);

void runCo2(void)
{
    printf("--------------\n");
    printf("Co2 begin to check!\n");

    rc = mh_z19_takeMeassuring();

    if (rc != MHZ19_OK)
    {
        printf("Fail on Co2 measuring\n");
    }
    else
    {
        printf("Co2 measurement: %d ppm\n", co2);
    }

    printf("--------------\n");
    printf("Co2 is set BIT_1!\n");
}

void co2Task(void *p)
{
    (void)p;

    co2_sensorInit();

    for (;;)
    {
        runCo2();
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

uint16_t getCo2(void)
{
    return co2;
}

void co2_sensorInit(void)
{
    mh_z19_initialise(ser_USART3);
    mh_z19_injectCallBack(co2CallBack);
}

void co2CallBack(uint16_t ppmCall)
{
    co2 = ppmCall;
}
