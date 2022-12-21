#include "freertos/portmacro.h"
#include "driver/i2s.h"
//Reference: https://dronebotworkshop.com/esp32-i2s/ 
//This file is for the handling of I2S Audio ONLY
//ARDUINO AND EVENTS MUST RUN ON CORE 1
#include "P2PConnect.h"
#include "I2S-Audio.h"
#include <Arduino.h>

//Mic Definitions
#define I2S_MIC_WS 25
#define I2S_MIC_SD 33
#define I2S_MIC_SCK 32

//Speaker Definitions
#define I2S_SPEAKER_DOUT 14

//Use I2S Processor 0
#define I2S_PORT I2S_NUM_0

#define bufferLen 64
int16_t sBuffer[bufferLen];

//Start multicore
TaskHandle_t I2SHandler;

void i2s_install() {
    //Set up I2S Processor configuration
    const i2s_config_t i2s_config = {
        .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX),
        .sample_rate = 16000,
        .bits_per_sample = i2s_bits_per_sample_t(16),
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = 0,
        .dma_buf_count = 4,
        .dma_buf_len = bufferLen,
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
  // DEMO CODE:
  // False print statements to "lock range" on serial plotter display
  // Change rangelimit value to adjust "sensitivity"
    for(;;) {
        // Get I2S data and place in data buffer
        size_t bytesIn = 0;
        esp_err_t result = i2s_read(I2S_PORT, &sBuffer, bufferLen, &bytesIn, portMAX_DELAY);

      if (result == ESP_OK)
      { 
        //Speaker Passthrough
        i2s_write(I2S_PORT, &sBuffer, bufferLen, &bytesIn, portMAX_DELAY);
      }
    }
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