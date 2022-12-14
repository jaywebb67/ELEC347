/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "uopmsb/uop_msb.h"
#include "goertzel.hpp"
#include <cstdint>
#include <cstdio>
#include <stdint.h>

// Blinking rate in milliseconds
#define Fs     8000

osThreadId_t mainThreadID;
Thread calcThread,outputThread;

Mutex bufferLock;

AnalogIn dataIn(MIC_AN_PIN);
Ticker sampleTick;

volatile int16_t bufferB[200];

void sampleISR();

void calcFreqMag(){

    goertzel lf1(697);
    goertzel lf2(770);
    goertzel lf3(852);
    goertzel lf4(941);

    goertzel hf1(1209);
    goertzel hf2(1336);
    goertzel hf3(1477);
    goertzel hf4(1633);
    while(true){

        if(!ThisThread::flags_wait_any_for(2,1s)){
            system_reset();
        }
        if(!bufferLock.trylock_for(1s)){
            system_reset();
        }
        uint32_t lfmag1 = lf1.detect(bufferB);
        uint32_t lfmag2 = lf2.detect(bufferB);
        uint32_t lfmag3 = lf3.detect(bufferB);
        uint32_t lfmag4 = lf4.detect(bufferB);
        uint32_t hfmag1 = hf1.detect(bufferB);
        uint32_t hfmag2 = hf2.detect(bufferB);
        uint32_t hfmag3 = hf3.detect(bufferB);
        uint32_t hfmag4 = hf4.detect(bufferB);
        printf("%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t",lfmag1,lfmag2,lfmag3,lfmag4,hfmag1,hfmag2,hfmag3,hfmag4);
        bufferLock.unlock();
        printf("\n\n\n");
        ThisThread::flags_clear(2); 
    }

}

int main() {
    uint16_t idx = 0;
    int16_t bufferA;


    calcThread.start(calcFreqMag);
    sampleTick.attach(&sampleISR,26ms);

    mainThreadID = ThisThread::get_id();
    while (true) {
        if(!ThisThread::flags_wait_any_for(1,1s)){
            system_reset();
        }
        if(!bufferLock.trylock_for(1s)){
            system_reset();
        }
        for (uint16_t i =0;i<200;i++){
            bufferA = dataIn.read_u16()-0x8000;
            bufferB[i] = bufferA;
            wait_us(125);
        };
        bufferLock.unlock();
        ThisThread::flags_clear(1);
        calcThread.flags_set(2);



        

        
    }
};


void sampleISR(){

    osSignalSet(mainThreadID,1);
};