#include "freertos/portmacro.h"
#include "driver/i2s.h"
//Reference: https://dronebotworkshop.com/esp32-i2s/ 
//This file is for the handling of I2S Audio ONLY
//ARDUINO AND EVENTS MUST RUN ON CORE 1
#include "P2PConnect.h"
#include "I2S-Audio.h"
#include "ExternalIO.h"
#include <Arduino.h>

#include "AudioBuffer.h"
#include "Statistics.h"

//Mic Definitions
#define I2S_MIC_WS 25
#define I2S_MIC_SD 33
#define I2S_MIC_SCK 32

//Speaker Definitions
#define I2S_SPEAKER_DOUT 14

//Use I2S Processor 0
#define I2S_PORT I2S_NUM_0
#define AUDIO_BUFFER_LENGTH 124

//Start multicore
TaskHandle_t I2SHandler;

// I2S configuration
// Set up I2S Processor configuration
const i2s_config_t i2s_config = {
    .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX),
    .sample_rate = 44100,
    .bits_per_sample = i2s_bits_per_sample_t(16),
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
    .intr_alloc_flags = 0,
    .dma_buf_count = 4,
    .dma_buf_len = 248,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0};

const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_MIC_SCK,
    .ws_io_num = I2S_MIC_WS,
    .data_out_num = I2S_SPEAKER_DOUT,
    .data_in_num = I2S_MIC_SD};

void i2s_init() {
  // Install I2S driver
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);

  // Set I2S pin configuration
  i2s_set_pin(I2S_NUM_0, &pin_config);

  // Start I2S
  i2s_start(I2S_NUM_0);
  i2s_zero_dma_buffer(I2S_PORT);    
}

void i2s_playback_task(void *pvParameters)
{
  Serial.printf("Playback task started on core %d\n", xPortGetCoreID());
  while(true)
  {
    size_t bytes_written = 0;
    int16_t audio_data[AUDIO_BUFFER_LENGTH];
    //while (true && !buttons[3].currentState == HIGH)
    while (true)
    {            
      if(!audioBuffer->read(audio_data, sizeof(audio_data)/ sizeof(int16_t)))
      {
         break;
      }
      // We play the audio sample on the speaker only if the talk button is not pressed
      if (!buttons[3].currentState == HIGH) 
      {
          i2s_write(I2S_PORT, audio_data, sizeof(audio_data), &bytes_written, portMAX_DELAY);
      }
      statistics.NumberOfSamplesPlayed += bytes_written / sizeof(int16_t);
      //statistics.NumberOfSamplesPlayed += 124;
    }

    statistics.PlayerTickRate++;
    vTaskDelay(1);
  }
}

void i2s_record_task(void *pvParameters)
{
  Serial.printf("Record task running on core: %d", xPortGetCoreID());

  bool isRecording = false;
  const uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  while (true)
  {
    int16_t audio_data[AUDIO_BUFFER_LENGTH];
    size_t bytes_read = 0;
    size_t bytes_written = 0;
    // Check if the Talk button is pressed. Record and broadcast audio only when so
    if (buttons[3].currentState == HIGH)
    {
      if (!isRecording)
      {
         Serial.println("Starting recording and broadcasting");
         // Send a start broadcasting message across the network 
         isRecording = true;
      }
      // Read audio data from the microphone
      i2s_read(I2S_PORT, audio_data, sizeof(audio_data), &bytes_read, portMAX_DELAY);

      // Broadcast audio data over the network immediatelly 
      esp_now_send(broadcastAddress, (const uint8_t *)audio_data, bytes_read);

      // Check if the Extra/PassThru button is pressed
      if (buttons[4].currentState == HIGH)
      {
         // Play the audio data over the speaker
         i2s_write(I2S_PORT, audio_data, sizeof(audio_data), &bytes_written, portMAX_DELAY);
      }

      statistics.NumberOfSamplesRecorded += bytes_read / sizeof(int16_t);
      statistics.NumberOfSamplesSent += bytes_read / sizeof(int16_t);
      statistics.BytesSent += bytes_read;
    }
    else
    {
      if (isRecording)
      {
         Serial.println("Stopping recording and broadcasting");
         isRecording = false;
         // Send one empty packet with 0xFF as the value. This tells the other side that we've finished sending
         i2s_zero_dma_buffer(I2S_PORT);
      }
      // Silence I2S when nothing is transmitting
      // i2s_zero_dma_buffer(I2S_PORT);
    }

    statistics.RecorderTickRate++;
    
    // Delay
    vTaskDelay(1);
  }
}

void i2s_start()
{
  xTaskCreate(i2s_record_task, "i2s_record_task", 8192, 0, 1, nullptr);
  xTaskCreate(i2s_playback_task, "i2s_playback_task", 8192, 0, 1, nullptr);

}