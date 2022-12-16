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
        int16_t testData[_N];
        float _targetFrequency[8] = {697,770,852,941,1209,1336,1477,1633};
        double coeff[8];
        double Q1;
        double Q2;
        double Q0;
        double omega[8];

        void ProcessSample(int sample,int i){


                Q2 = Q1;
                Q1 = Q0;
                Q0 = coeff[i] * Q1 - Q2 + float(sample);


            
        }
	    void ResetGoertzel(){
            Q2 = 0;
            Q1 = 0;
            Q0 = 0;

        }

    public:

        float magnitude[8];

        goertzel(AnalogIn dataPin) : dataIn(dataPin){
            for (int i=0;i<8;i++){
                omega[i] = (2.0 * PI * _targetFrequency[i] ) / _samplingFrequency;
                coeff[i] = 2.0 * cos(omega[i]);
                magnitude[i] = 0;
            }
            ResetGoertzel();

        }

        void sample(){

            for(int i=0; i<200;i++){
                testData[i] = dataIn.read_u16()-(0x8000);
                wait_us(125);
            };


        }

	    void detect() {
    
            for(int i=0; i<8;i++){
                /* Process the samples. */
                for (int index = 0; index < _N; index++)
                {
                    ProcessSample(testData[index],i);
                }

                /* Do the "optimized Goertzel" processing. */
                magnitude[i] = sqrt(Q1*Q1 + Q2*Q2 - coeff[i]*Q1*Q2);

                ResetGoertzel();
            }
            

        };
};

#endif