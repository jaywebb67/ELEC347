/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "uopmsb/uop_msb.h"
//#include "goertzel.hpp"
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <stdint.h>


// Blinking rate in milliseconds
#define Fs     8000
#define PI 3.14159265358979323846
#define _N 200

osThreadId_t mainThreadID;
Thread calcThread;

Mutex bufferLock;

AnalogIn dataIn(MIC_AN_PIN);
Ticker sampleTick;

volatile uint16_t samples[200];

void sampleISR();

 void calcFreqMag(){

    volatile float k;
    volatile float floatN;
    volatile float omega,sine,cosine,coeff,Q0,Q1,Q2,magnitude[8],real,imag;

    volatile float scalingFactor = _N/2.0;
    floatN = (float) _N;
    while(true){

        if(!ThisThread::flags_wait_any_for(2,10s)){
            system_reset();
        }
        if(!bufferLock.trylock_for(10s)){
            system_reset();
        }
        int freq_centre[8] = {697,770,852,941,1209,1336,1477,1633};
        for (int i = 0; i<8;i++){

            k = (0.5 + ((floatN * freq_centre[i]) / Fs));
            omega = (2.0 * PI * k) / floatN;
            sine = sin(omega);
            cosine = cos(omega);
            coeff = 2.0 * cosine;
        
            Q0 = 0;
            Q1 = 0;
            Q2 = 0;
            for (uint16_t index = 0; index < _N; index++)
            {
                Q0 = coeff * Q1 - Q2 + (float)samples[index];
                Q2 = Q1;
                Q1 = Q0;
            }
            // calculate the real and imaginary results
            // scaling appropriately
            real = (Q1 - Q2 * cosine) / scalingFactor;
            imag = (Q2 * sine) / scalingFactor;
            magnitude[i] = sqrtf(real*real + imag*imag);
            cout << magnitude[i] << endl;
            Q1 = 0;
            Q2 = 0;
        }
        bufferLock.unlock();

        printf("\n\n\n");
        ThisThread::flags_clear(2); 
        wait_us(5000000);
        sampleTick.attach(&sampleISR,125us);    
    }

}

int main() {
    int idx = 0;
    uint16_t sample;


    calcThread.start(calcFreqMag);
    sampleTick.attach(&sampleISR,125us);

    mainThreadID = ThisThread::get_id();
    while (true) {
        if(!ThisThread::flags_wait_any_for(1,10s)){
            system_reset();
        }
        if(!bufferLock.trylock_for(10s)){
            system_reset();
        }
        sample = dataIn.read_u16()-0x8000;
        samples[idx] = sample;
        bufferLock.unlock();
        if(idx>=199){
            sampleTick.detach();
            calcThread.flags_set(2);
            idx = 0;
            //printf("\n\n\n");
            //wait_us(5000000);

        }
        else {
            idx = idx + 1; 
        }
        //ThisThread::flags_clear(1);
     
        //printf("%d\t",idx);
        
    }
};


void sampleISR(){

    osSignalSet(mainThreadID,1);
};