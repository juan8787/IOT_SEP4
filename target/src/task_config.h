#pragma once

/* Priorities at which the tasks are created. */
#define TEMP_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define HUMIDITY_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define CO2_TASK_PRIORITY (tskIDLE_PRIORITY + 1)
#define CONTROLLER_TASK_PRIORITY (tskIDLE_PRIORITY + 2)

/* Task stack sizes*/
#define TEMP_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE)
#define HUMIDITY_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE)
#define CO2_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE)
#define CONTROLLER_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE)