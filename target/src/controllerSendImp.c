#include <stdio.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "event_groups.h"
#include <queue.h>
#include "sensors/temperature.h"
#include "sensors/co2.h"
#include "sensors/humidity.h"
#include "controllerSend.h"

sensors_data_t dataC;
QueueHandle_t xQueue2;
extern EventGroupHandle_t EventGroupHandle;
#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)

void runControllerSend()
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
    printf("ControllerSend start to set the data\n");

    // Set the data fields from the sensors using pointers
    sensors_data_t *pData = &dataC;
    pData->co2 = getCo2();
    pData->humidity = getHumidity();
    pData->temperature = getTemperature();
    // xQueueSend(xQueue2, (void*)&dataC,0);
    // Set timeout in queue so if its full it wont be blocked forever, but rather for only 1 milisecond
    if (xQueueSendToBack(xQueue2, (void *)&dataC, 1) != pdPASS)
    {
        printf("queue is full");
        //(queue is full), ignore and lose the packet.
    }
    else
    {
        printf("Send data to queue");
    }
}

void controllerSendTask(void *p)
{

    (void)p;
    xQueue2 = xQueueCreate(1, sizeof(dataC));
    for (;;)
    {
        runControllerSend();
    }
}
