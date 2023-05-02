#include <stdio.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include "temperature.h"
#include <hih8120.h>

extern EventGroupHandle_t EventGroupHandle;
#define BIT_0 (1 << 0)

static uint16_t temp = 0;

void runTemp()
{
    printf("--------------\n");
    printf("Temp begin to check!\n");

    for (int i = 0; i < 10; i++)
    {
        if (HIH8120_OK == hih8120_wakeup())
        {
            vTaskDelay(pdMS_TO_TICKS(50));
            if (HIH8120_OK == hih8120_measure())
            {
                vTaskDelay(pdMS_TO_TICKS(1));
                temp = temp + hih8120_getTemperature_x10();
            }
        }
        else
        {
            printf("Temperature sensor could not wakeup");
        }
        vTaskDelay(pdMS_TO_TICKS(25000));
    }

    xEventGroupSetBits(EventGroupHandle, BIT_0);
    printf("--------------\n");
    printf("temperature is set BIT_0!\n");
    vTaskDelay(pdMS_TO_TICKS(50));
}

void temperatureTask(void *p)
{
    (void)p;

    for (;;)
    {
        runTemp();
    }
}

uint16_t getTemperature()
{
    uint16_t *restartCo2 = &temp;
    uint16_t avgCo2 = temp;
    *restartCo2 = 0;

    return avgCo2 / 10;
}
void tempSensorInit()
{
    if (HIH8120_OK != hih8120_initialise())
    {
        printf("Temperature sensor could not initialized");
    }
}
