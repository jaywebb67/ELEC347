/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "uopmsb/uop_msb.h"
#include "goertzel.hpp"
#include <cstdint>
#include <stdint.h>

// Blinking rate in milliseconds
#define Fs     8000
// #define mic_pin PA_3

osThreadId_t mainThreadID;

AnalogIn dataIn(MIC_AN_PIN);
Ticker sampleTick;

void sampleISR();


int main() {
    int idx = 0;
    int16_t bufferA;
    int16_t bufferB[200];
    goertzel data;

    sampleTick.attach(&sampleISR,26ms);

    mainThreadID = ThisThread::get_id();
    while (true) {
        ThisThread::flags_wait_any(1);
        
        for (int i =0;i<200;i++){
            bufferA = dataIn.read_u16()-0x8000;
            bufferB[idx] = bufferA;
            wait_us(125);
        }
        
        ThisThread::flags_clear(1);
        printf("\n\n\n\n");

        

        
    }
};


void sampleISR(){

    osSignalSet(mainThreadID,1);
};