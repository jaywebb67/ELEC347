/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "uopmsb/uop_msb.h"
#include "goertzel.hpp"
//#include "math.h"

using namespace uop_msb;

//define threshold for deteciton
#define threshold 150000

//make 7 segment object to display the numbers 
LatchedLED sev_seg(LatchedLED::SEVEN_SEG,LatchedLED::UNITS);

int main() {

    goertzel data(MIC_AN_PIN);
    float targetFrequency[8] = {697,770,852,941,1209,1336,1477,1633};
    while (true) {
        
        //collects 205 samples at the sampling frequency
        data.sample();

        //runs algorithm to obtain magnitudes
        data.detect();


        //the main checking process, only allows for one key if over the given threshold
        if ((data.magnitude[0]>threshold)&(data.magnitude[0]<data.magnitude[1])&(data.magnitude[0]>data.magnitude[2])&(data.magnitude[0]>data.magnitude[3])){
            if ((data.magnitude[4]>threshold)&(data.magnitude[4]<data.magnitude[5])&(data.magnitude[4]>data.magnitude[6])&(data.magnitude[4]>data.magnitude[7])){
                sev_seg.enable(1);
                sev_seg = 1;
            }
            else if ((data.magnitude[5]>threshold)&(data.magnitude[5]>data.magnitude[4])&(data.magnitude[5]<data.magnitude[6])&(data.magnitude[5]>data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 2;
            }
            else if ((data.magnitude[6]>threshold)&(data.magnitude[6]>data.magnitude[4])&(data.magnitude[5]<data.magnitude[6])&(data.magnitude[6]<data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 3;
            }
            else if ((data.magnitude[7]>threshold)&(data.magnitude[7]>data.magnitude[4])&(data.magnitude[7]>data.magnitude[6])&(data.magnitude[5]<data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 10;
            }
        }
        else if ((data.magnitude[1]>threshold)&(data.magnitude[0]<data.magnitude[1])&(data.magnitude[1]<data.magnitude[2])&(data.magnitude[1]>data.magnitude[3])){
            if ((data.magnitude[4]>threshold)&(data.magnitude[4]<data.magnitude[5])&(data.magnitude[4]>data.magnitude[6])&(data.magnitude[4]>data.magnitude[7])){
                sev_seg.enable(1);
                sev_seg = 4;
            }
            else if ((data.magnitude[5]>threshold)&(data.magnitude[5]>data.magnitude[4])&(data.magnitude[5]<data.magnitude[6])&(data.magnitude[5]>data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 5;
            }
            else if ((data.magnitude[6]>threshold)&(data.magnitude[6]>data.magnitude[4])&(data.magnitude[5]<data.magnitude[6])&(data.magnitude[6]<data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 6;
            }
            else if ((data.magnitude[7]>threshold)&(data.magnitude[7]>data.magnitude[4])&(data.magnitude[7]>data.magnitude[6])&(data.magnitude[5]<data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 11;
            }
        }
        else if ((data.magnitude[2]>threshold)&(data.magnitude[2]>data.magnitude[1])&(data.magnitude[0]<data.magnitude[2])&(data.magnitude[2]<data.magnitude[3])){
            if ((data.magnitude[4]>threshold)&(data.magnitude[4]<data.magnitude[5])&(data.magnitude[4]>data.magnitude[6])&(data.magnitude[4]>data.magnitude[7])){
                sev_seg.enable(1);
                sev_seg = 7;
            }
            else if ((data.magnitude[5]>threshold)&(data.magnitude[5]>data.magnitude[4])&(data.magnitude[5]<data.magnitude[6])&(data.magnitude[5]>data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 8;
            }
            else if ((data.magnitude[6]>threshold)&(data.magnitude[6]>data.magnitude[4])&(data.magnitude[5]<data.magnitude[6])&(data.magnitude[6]<data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 9;
            }
            else if ((data.magnitude[7]>threshold)&(data.magnitude[7]>data.magnitude[4])&(data.magnitude[7]>data.magnitude[6])&(data.magnitude[5]<data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 12;
            }
        }
        else if ((data.magnitude[3]>threshold)&(data.magnitude[3]>data.magnitude[1])&(data.magnitude[3]>data.magnitude[2])&(data.magnitude[0]<data.magnitude[3])){
            if ((data.magnitude[4]>threshold)&(data.magnitude[4]<data.magnitude[5])&(data.magnitude[4]>data.magnitude[6])&(data.magnitude[4]>data.magnitude[7])){
                sev_seg.enable(1);
                sev_seg = 14;
            }
            else if ((data.magnitude[5]>threshold)&(data.magnitude[5]>data.magnitude[4])&(data.magnitude[5]<data.magnitude[6])&(data.magnitude[5]>data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 0;
            }
            else if ((data.magnitude[6]>threshold)&(data.magnitude[6]>data.magnitude[4])&(data.magnitude[5]<data.magnitude[6])&(data.magnitude[6]<data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 15;
            }
            else if ((data.magnitude[7]>threshold)&(data.magnitude[7]>data.magnitude[4])&(data.magnitude[7]>data.magnitude[6])&(data.magnitude[5]<data.magnitude[7])) {
                sev_seg.enable(1);
                sev_seg = 13;
            }
        }
        wait_us(1000000);
        sev_seg.enable(0);
    }
};


