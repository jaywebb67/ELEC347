/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "uopmsb/uop_msb.h"
#include "goertzel.hpp"






int main() {

    goertzel data(MIC_AN_PIN);
    float targetFrequency[8] = {697,770,852,941,1209,1336,1477,1633};
    while (true) {
     
        data.sample();
        data.detect();
        for (int i = 0; i<8; i++){

            printf("%lf\t",data.magnitude[i]);
        };


        wait_us(1000000);
    }
};


