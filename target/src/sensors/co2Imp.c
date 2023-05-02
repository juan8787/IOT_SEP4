#include <stdio.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include "co2.h"
#include <mh_z19.h>

extern EventGroupHandle_t EventGroupHandle;
#define BIT_1 (1 << 1)
static mh_z19_returnCode_t rc;
static uint16_t co2 = 0;
void runCo2()
{

    printf("--------------\n");
    printf("Co2 begin to check!\n");
    vTaskDelay(pdMS_TO_TICKS(200000));
    rc = mh_z19_takeMeassuring();

    if (rc != MHZ19_OK)
    {
        printf("Fail on Co2 messuring");
    }

    xEventGroupWaitBits(
        EventGroupHandle,
        BIT_1,
        pdTRUE,
        pdTRUE,
        portMAX_DELAY);

    printf("--------------\n");
    printf("Co2  is set BIT_1!\n");
}

void co2Task(void *p)
{
    (void)p;

    co2_sensorInit();
    for (;;)
    {
        runCo2();
    }
}

uint16_t getCo2()
{
    uint16_t *restartCo2 = &co2;
    uint16_t avgCo2 = co2;
    *restartCo2 = 0;
    return avgCo2 / 5;
}

void co2_sensorInit()
{
    mh_z19_initialise(ser_USART3);
    mh_z19_injectCallBack(co2CallBack);
}
void co2CallBack(uint16_t ppmCall)
{
    co2 = ppmCall;
}