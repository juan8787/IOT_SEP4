#pragma once
#include <stdint.h>

typedef struct sensors_data
{
    int16_t temperature;
    uint16_t humidity;
    uint16_t co2;
} sensors_data_t;

void environmentTask(void *p);
