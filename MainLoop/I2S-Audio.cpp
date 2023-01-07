#include "freertos/portmacro.h"
#include "driver/i2s.h"
//Reference: https://dronebotworkshop.com/esp32-i2s/ 
//This file is for the handling of I2S Audio ONLY
//ARDUINO AND EVENTS MUST RUN ON CORE 1
#include "P2PConnect.h"
#include "I2S-Audio.h"
#include "ExternalIO.h"
#include <Arduino.h>

//Mic Definitions
#define I2S_MIC_WS 25
#define I2S_MIC_SD 33
#define I2S_MIC_SCK 32

//Speaker Definitions
#define I2S_SPEAKER_DOUT 14

//Use I2S Processor 0
#define I2S_PORT I2S_NUM_0
#define bufferLength 128
int8_t outgoingBuffer[128];

//Start multicore
TaskHandle_t I2SHandler;

void i2s_install() {
    //Set up I2S Processor configuration
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX),
        .sample_rate = 48000,
        .bits_per_sample = i2s_bits_per_sample_t(16),
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = 0,
        .dma_buf_count = 4,
        .dma_buf_len = bufferLength,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
}

void i2s_setpin() {
  // Set I2S pin configuration
  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_MIC_SCK,
    .ws_io_num = I2S_MIC_WS,
    .data_out_num = I2S_SPEAKER_DOUT,
    .data_in_num = I2S_MIC_SD
  };

  i2s_set_pin(I2S_PORT, &pin_config);
}

void i2s_init() {
    i2s_install();
    i2s_setpin();
    i2s_start(I2S_PORT);
}

void I2SHandlerSRC(void*pvParameters) {
  Serial.print("I2S running on core: ");
  Serial.println(xPortGetCoreID());
  //Loop audio process
  for(;;) {
    size_t bytesIn = 0;
    // Buffer received from peer
    if (bufferReceived) {
      i2s_write(I2S_PORT, &incomingBuffer, incomingBufferLength, &bytesIn, portMAX_DELAY);
    }

    // Only Read if talk button is pressed
    // Get I2S data and place in data buffer
    else if (buttons[3].currentState == HIGH) {
      esp_err_t readResult = i2s_read(I2S_PORT, &outgoingBuffer, bufferLength, &bytesIn, portMAX_DELAY);
      if (readResult == ESP_OK) { 
        // Extra Button
        if (buttons[4].currentState == HIGH){
          // Speaker Passthrough Debug
          i2s_write(I2S_PORT, &outgoingBuffer, bufferLength, &bytesIn, portMAX_DELAY);
        }

        else {
          //Silence the TX buffer
           i2s_zero_dma_buffer(I2S_PORT);
        }
      }
    } 
    
    else {
        //Silence I2S when nothing is transmitting
        i2s_zero_dma_buffer(I2S_PORT);
    }
    //Stop watchdog timer
    vTaskDelay(1);
  }
}

void i2s_startTask() {
    xTaskCreatePinnedToCore(
        I2SHandlerSRC,
        "I2SHandler",
        10000,
        NULL,
        1,
        &I2SHandler,
        0);
}