/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "uopmsb/uop_msb.h"
#include "goertzel.hpp"

using namespace uop_msb;

#define threshold 150000

LatchedLED sev_seg(LatchedLED::SEVEN_SEG,LatchedLED::UNITS);

int main() {

    goertzel data(MIC_AN_PIN);
    float targetFrequency[8] = {697,770,852,941,1209,1336,1477,1633};
    while (true) {
     
        data.sample();
        data.detect();

        if ((data.magnitude[0]>threshold)&(data.magnitude[1]>threshold)&(data.magnitude[4]>threshold)){
            sev_seg.enable(1);
            sev_seg = 1;
        }
        else if ((data.magnitude[1]>threshold)&(data.magnitude[2]>threshold)&(data.magnitude[4]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 4;
        }
        else if ((data.magnitude[2]>threshold)&(data.magnitude[3]>threshold)&(data.magnitude[4]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 7;
        }   
        else if ((data.magnitude[2]<threshold)&(data.magnitude[3]>threshold)&(data.magnitude[4]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 10;
        } 
        else if ((data.magnitude[0]>threshold)&(data.magnitude[1]>threshold)&(data.magnitude[5]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 2;
        } 
        else if ((data.magnitude[1]>threshold)&(data.magnitude[2]>threshold)&(data.magnitude[5]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 5;
        } 
        else if ((data.magnitude[2]>threshold)&(data.magnitude[3]>threshold)&(data.magnitude[5]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 8;
        } 
        else if ((data.magnitude[2]<threshold)&(data.magnitude[3]>threshold)&(data.magnitude[5]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 0;
        } 
        else if ((data.magnitude[0]>threshold)&(data.magnitude[1]>threshold)&(data.magnitude[6]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 3;
        } 
        else if ((data.magnitude[1]>threshold)&(data.magnitude[2]>threshold)&(data.magnitude[6]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 6;
        } 
        else if ((data.magnitude[2]>threshold)&(data.magnitude[3]>threshold)&(data.magnitude[6]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 9;
        } 
        else if ((data.magnitude[2]<threshold)&(data.magnitude[3]>threshold)&(data.magnitude[6]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 11;
        }
        else if ((data.magnitude[0]>threshold)&(data.magnitude[1]>threshold)&(data.magnitude[7]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 12;
        } 
        else if ((data.magnitude[1]>threshold)&(data.magnitude[2]>threshold)&(data.magnitude[7]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 13;
        } 
        else if ((data.magnitude[2]>threshold)&(data.magnitude[3]>threshold)&(data.magnitude[7]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 14;
        } 
        else if ((data.magnitude[2]<threshold)&(data.magnitude[3]>threshold)&(data.magnitude[7]>threshold)) {
            sev_seg.enable(1);
            sev_seg = 15;
        }

        wait_us(1000000);
        sev_seg.enable(0);
    }
};


