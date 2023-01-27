#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <algorithm>

class AudioBuffer {
public:
    static constexpr size_t NUMBER_OF_SAMPLES = 44100;
    static constexpr size_t BUFFER_SIZE = NUMBER_OF_SAMPLES;

    AudioBuffer() {
        buffer = (int16_t*)malloc(BUFFER_SIZE * sizeof(int16_t));
        memset(buffer, 0, BUFFER_SIZE * sizeof(int16_t));
        read_pos = 0;
        write_pos = 0;
        sample_count = 0;
        semaphore = xSemaphoreCreateBinary();
        xSemaphoreGive(semaphore);
        xTaskCreate(task, "abst", 2048, this, 5, &stat_task_handle);
    }

    ~AudioBuffer() {
        vTaskDelete(stat_task_handle);
        vSemaphoreDelete(semaphore);
        free(buffer);
    }

    bool write(int16_t* data, size_t len) {
        if( xSemaphoreTake(semaphore, portMAX_DELAY) != pdTRUE)
        {
            return false;
        }

        if (sample_count + len > BUFFER_SIZE) {
            len = BUFFER_SIZE - sample_count;
            xSemaphoreGive(semaphore);
            return false;
        }


        size_t first_part_len = min(len, BUFFER_SIZE - write_pos);
        memcpy(buffer + write_pos, data, first_part_len * sizeof(int16_t));

        if (first_part_len < len) {
            memcpy(buffer, data + first_part_len, (len - first_part_len) * sizeof(int16_t));
        }

        write_pos = (write_pos + len) % BUFFER_SIZE;

        sample_count += len;

        xSemaphoreGive(semaphore);
        return true;
    }
    
    bool read(int16_t* data, size_t len) {

        if( xSemaphoreTake(semaphore, portMAX_DELAY) != pdTRUE)
        {
            return  false;
        }

        if (len > sample_count) {
            len = sample_count;
            xSemaphoreGive(semaphore);
            return false;
        }

        size_t first_part_len = min(len, BUFFER_SIZE - read_pos);
        memcpy(data, buffer + read_pos, first_part_len * sizeof(int16_t));

        if (first_part_len < len) {
            memcpy(data + first_part_len, buffer, (len - first_part_len) * sizeof(int16_t));
        }

        read_pos = (read_pos + len) % BUFFER_SIZE;

        sample_count -= len;

        xSemaphoreGive(semaphore);
        return true;
    }
    
    int count() {
        return sample_count;
    }

private:
    int16_t* buffer;
    size_t read_pos;
    size_t write_pos;
    size_t sample_count;
    SemaphoreHandle_t semaphore;
    TaskHandle_t stat_task_handle;
    static void task(void* pvParameters) {
        AudioBuffer* buf = (AudioBuffer*)pvParameters;
        while (1) {
            printf("Audio Bufer Usage: %d/%d samples (%.2f%%)\n", buf->count(), BUFFER_SIZE, (buf->count() * 100.0) / BUFFER_SIZE);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
};

extern AudioBuffer* audioBuffer;
