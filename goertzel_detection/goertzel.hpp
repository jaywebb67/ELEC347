#ifndef goertzel_hpp
#define goertzel_hpp


#include "mbed.h"
#include <cstdint>
#include "math.h"

#define _samplingFrequency 8000
#define _N 200
#define PI 3.14159265358979323846

class goertzel {

    private:

        AnalogIn dataIn;
        float testData[_N];
        float _targetFrequency[8] = {697,770,852,941,1209,1336,1477,1633};
        double coeff[8];
        double Q1;
        double Q2;
        double Q0;
        double omega[8];

        void ProcessSample(int sample,int i){

                Q0 = coeff[i] * Q1 - Q2 + float(sample);
                Q2 = Q1;
                Q1 = Q0;
            
        }
	    void ResetGoertzel(){
            Q2 = 0;
            Q1 = 0;

        }

    public:


        goertzel(AnalogIn dataPin) : dataIn(dataPin){
            for (int i=0;i<8;i++){
                omega[i] = (2.0 * PI * _targetFrequency[i] ) / _samplingFrequency;
                coeff[i] = 2.0 * cos(omega[i]);
            }
            ResetGoertzel();

        }

        void sample(){

            for(int i=0; i<200;i++){
                testData[i] = dataIn.read()-(32768.0/65536.0);
                wait_us(125);
            };


        }

	    float detect(int i) {
            float magnitude;

            /* Process the samples. */
            for (uint16_t index = 0; index < _N; index++)
            {
                ProcessSample(testData[index],i);
            }

            /* Do the "optimized Goertzel" processing. */
            magnitude = sqrt(Q1*Q1 + Q2*Q2 - coeff[i]*Q1*Q2);

            ResetGoertzel();
            return magnitude;
            

        };
};

#endif