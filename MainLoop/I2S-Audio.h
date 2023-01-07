#ifndef I2S_AUDIO_H
#define I2S_AUDIO_H
#include <Arduino.h>
#include <driver/i2s.h>

extern int8_t sBuffer[128];

void i2s_install();
//Starts I2S Driver

void i2s_setpin();
//Sets I2S Pin

void i2s_init();
//Init I2S

void I2SHandlerSRC(void*pvParameters);
//Source code for task to run

void i2s_startTask();
//Start multicore task

#endif