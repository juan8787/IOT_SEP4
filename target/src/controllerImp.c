#include <stdio.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"

#include "sensors/temperature.h"
#include "sensors/co2.h"
#include "sensors/humidity.h"
#include "controller.h"

sensors_data_t dataC;
extern EventGroupHandle_t EventGroupHandle;
#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
#define BIT_4 (1 << 4)

void runEnvironment()
{
    xEventGroupWaitBits(
        EventGroupHandle,
        BIT_0 | BIT_1 | BIT_2,
        pdTRUE,
        pdTRUE,
        portMAX_DELAY);

    vTaskDelay(40);
    printf("--------------\n");
    printf("--------------\n");
    printf("Environment start to set the data\n");

    // Set the data fields from the sensors using pointers
    sensors_data_t *pData = &dataC;
    pData->co2 = getCo2();
    pData->humidity = getHumidity();
    pData->temperature = getTemperature();

    xEventGroupSetBits(EventGroupHandle, BIT_0 | BIT_1 | BIT_2);
}

void environmentTask(void *p)
{

    (void)p;
    for (;;)
    {
        runEnvironment();
    }
}
