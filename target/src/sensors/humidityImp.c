#include <stdio.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include "humidity.h"
#include <hih8120.h>

extern EventGroupHandle_t EventGroupHandle;
#define BIT_2 (1 << 2)

static uint16_t humidity = 0;
void runHumidity()
{

    printf("--------------\n");
    printf("humidity begin to check!\n");
    for (int i = 0; i < 10; i++)
    {
        if (HIH8120_OK == hih8120_wakeup())
        {
            vTaskDelay(pdMS_TO_TICKS(50));
            if (HIH8120_OK == hih8120_measure())
            {
                vTaskDelay(pdMS_TO_TICKS(1));
                humidity = humidity + hih8120_getHumidityPercent_x10();
            }
        }
        else
        {
            printf("Humidity sensor could not wakeup");
        }

        vTaskDelay(pdMS_TO_TICKS(25000));
    }

    xEventGroupWaitBits(
        EventGroupHandle,
        BIT_2,
        pdTRUE,
        pdTRUE,
        portMAX_DELAY);
    printf("--------------\n");
    printf("humidity  is set BIT_2!\n");
    vTaskDelay(50);
}
void humiditySensorInit()
{
    if (HIH8120_OK != hih8120_initialise())
    {
        printf("Humidity sensor could not initialized");
    }
}
void humidityTask(void *p)
{
    (void)p;
    humiditySensorInit();

    for (;;)
    {
        runHumidity();
    }
}

uint16_t getHumidity()
{
    uint16_t *restartHumidity = &humidity;
    uint16_t avgHumidity = humidity;
    *restartHumidity = 0;
    return avgHumidity / 10;
}
