#include "Statistics.h"

statistics_t statistics;

void statistics_task(void *pvParameters)
{
    while (true)
    {
        printf("Statistics | NumberOfSamplesSent=%d NumberOfSamplesReceived=%d NumberOfSamplesPlayed=%d NumberOfSamplesRecorded=%d BytesSent=%d BytesReceived=%d RecorederTickRate=%d PlayerTickRate=%d\n",
               statistics.NumberOfSamplesSent, statistics.NumberOfSamplesReceived, statistics.NumberOfSamplesPlayed, statistics.NumberOfSamplesRecorded, statistics.BytesSent, statistics.BytesReceived, statistics.RecorderTickRate, statistics.PlayerTickRate);
        statistics.NumberOfSamplesSent = 0;
        statistics.NumberOfSamplesReceived = 0;
        statistics.NumberOfSamplesPlayed = 0;
        statistics.NumberOfSamplesRecorded = 0;
        statistics.BytesSent = 0;
        statistics.BytesReceived = 0;
        statistics.RecorderTickRate = 0;
        statistics.PlayerTickRate = 0;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void statistics_init()
{
    statistics.NumberOfSamplesSent = 0;
    statistics.NumberOfSamplesReceived = 0;
    statistics.NumberOfSamplesPlayed = 0;
    statistics.NumberOfSamplesRecorded = 0;
    statistics.BytesSent = 0;
    statistics.BytesReceived = 0;
    statistics.RecorderTickRate = 0;
    statistics.PlayerTickRate = 0;
    xTaskCreate(statistics_task, "statistics_task", 2048, 0, tskIDLE_PRIORITY, nullptr);
}
