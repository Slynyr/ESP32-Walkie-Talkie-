#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

typedef struct statistics_t
{
    int NumberOfSamplesSent;
    int NumberOfSamplesReceived;
    int NumberOfSamplesPlayed;
    int NumberOfSamplesRecorded;
    int BytesSent;
    int BytesReceived;
    int RecorderTickRate;
    int PlayerTickRate;
};

extern statistics_t statistics;
void statistics_init();
