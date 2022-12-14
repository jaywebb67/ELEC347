#ifndef goertzel_hpp
#define goertzel_hpp


#include "mbed.h"
#include <cstdint>
#include "math.h"

#define _samplingFrequency 8000
#define _N 200
#define PI 3.14

class goertzel {




    private:
        double freq_centres[8] = {697,770,852,941,1209,1336,1477,1633};

        double _targetFrequency;
        double coeff;
        double Q1;
        double Q2;
        double Q0;
        double omega;

        void ProcessSample(int sample){

                Q0 = coeff * Q1 - Q2 + float(sample);
                Q2 = Q1;
                Q1 = Q0;
            
        }
	    void ResetGoertzel(){
            Q2 = 0;
            Q1 = 0;
            Q0 = 0;
        }

    public:


        goertzel(double targetFrequency) : _targetFrequency(targetFrequency){

                omega = (2.0 * PI * _targetFrequency ) / _samplingFrequency;
                coeff = 2.0 * cos(omega);

            ResetGoertzel();

        }

	    double detect(volatile int16_t testData[_N]) {
            double magnitude;
            ResetGoertzel();
            /* Process the samples. */
            for (uint16_t index = 0; index < _N; index++)
            {
                ProcessSample(testData[index]);
            }

            /* Do the "optimized Goertzel" processing. */
            return magnitude = sqrt(Q1*Q1 + Q2*Q2 - coeff*Q1*Q2);


            

        };
};

#endif