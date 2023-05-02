/*
 * main.c
 * Author : IHA
 *
 * Example main file including LoRaWAN setup
 * Just for inspiration :)
 */

#include <stdio.h>
#include <avr/io.h>

#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "event_groups.h"

#include <stdio_driver.h>
#include <serial.h>

#include "sensors/temperature.h"
#include "sensors/co2.h"
#include "sensors/humidity.h"
#include "controller.h"
#include "task_config.h"

// Needed for LoRaWAN
#include <lora_driver.h>
#include <status_leds.h>

// define two Tasks
void task1(void *pvParameters);
void task2(void *pvParameters);

EventGroupHandle_t EventGroupHandle = NULL;

// Prototype for LoRaWAN handler
void lora_handler_initialise(UBaseType_t lora_handler_task_priority);

/*-----------------------------------------------------------*/
void create_tasks(void)
{

	xTaskCreate(temperatureTask, "temp_Task", TEMP_TASK_STACK_SIZE, NULL, TEMP_TASK_PRIORITY, NULL);
	xTaskCreate(co2Task, "co2_Task", CO2_TASK_STACK_SIZE, NULL, CO2_TASK_PRIORITY, NULL);
	xTaskCreate(humidityTask, "humidity_Task", HUMIDITY_TASK_STACK_SIZE, NULL, HUMIDITY_TASK_PRIORITY, NULL);
	xTaskCreate(environmentTask, "environment_Task", CONTROLLER_TASK_STACK_SIZE, NULL, CONTROLLER_TASK_PRIORITY, NULL);
}

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
void initialiseSystem()
{
	// inicialise EventGroup
	EventGroupHandle = xEventGroupCreate();
	if (EventGroupHandle == NULL)
	{
		printf("Failed to create EventGroup\n");
		return -1;
	}
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdio_initialise(ser_USART0);
	// Let's create some tasks
	create_tasks();

	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Status Leds driver
	status_leds_initialise(5); // Priority 5 for internal task
	// Initialise the LoRaWAN driver without down-link buffer
	lora_driver_initialise(1, NULL);
	// Create LoRaWAN task and start it up with priority 3
	lora_handler_initialise(3);
}

/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	printf("Program Started!!\n");
	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.

	/* Replace with your application code */
	while (1)
	{
	}
}
