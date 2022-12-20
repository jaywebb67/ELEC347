#ifndef goertzel_hpp
#define goertzel_hpp


#include "mbed.h"
#include <cstdint>
#include "math.h"

#define _samplingFrequency 8000
#define _N 205
#define PI 3.14159265358979323846

class goertzel {

    private:
        //member objects used for the algorithm 
        AnalogIn dataIn;
        int16_t testData[_N];
        float _targetFrequency[8] = {697,770,852,941,1209,1336,1483,1639};
        double coeff[8];
        double lf_Vk;
        double lf_Vk_1;
        double lf_Vk_2;
        double omega[8];
        double hf_Vk[_N+2];
        double hf_Vk_1;
        double hf_Vk_2;
        int k[8];
        double cosine[8];
        float ref_volt;

        //used to process the sample for each iteration, 0 through N
        void ProcessSample_lf(int sample,int i, int index){   
            lf_Vk = coeff[i] * lf_Vk_1 - lf_Vk_2 + sample;
            lf_Vk_2 = lf_Vk_1;
            lf_Vk_1 = lf_Vk;

        }

        //used to process the sample for each iteration, 0 through N
        void ProcessSample_hf(int sample,int i, int index){   

            hf_Vk[index] = coeff[i] * hf_Vk[index-1] - hf_Vk[index-2] + sample;

        }

        //function is used initial conditions to 0
	    void ResetGoertzel(){

            hf_Vk[1] = 0;
            hf_Vk[0] = 0;
            lf_Vk_1 = 0;
            lf_Vk_2 = 0;
            lf_Vk = 0;

        }

    public:
        //public member object to store magnitudes in
        float magnitude[8];


        //class constructor, defines all the pre-computed constants needed for the algorithm
        goertzel(AnalogIn dataPin) : dataIn(dataPin){
            dataIn.set_reference_voltage(1);
            for (int i=0;i<8;i++){
                k[i] = int(round(0.5 + (_N * _targetFrequency[i]) / _samplingFrequency ));
                omega[i] = (2.0 * PI * k[i] ) / _N;
                cosine[i] = cos(omega[i]);
                coeff[i] = 2.0 * cosine[i];
                magnitude[i] = 0;
            }
            ResetGoertzel();

        }


        //function that performs the sampling and stores into an array
        void sample(){

            for(int i=0; i<200;i++){
                testData[i] = dataIn.read_u16();//-(0x8000);
                wait_us(125);
            };
        }

        //function that performs the algorithm and returns the magnitudes 
	    void detect() {
    
            for(int i=0; i<8;i++){
                /* Process the samples. */
                for (int index = 2; index <= _N+2; index++)
                {
                    //ProcessSample_lf(testData[index],i,index);
                   ProcessSample_hf(testData[index-2],i,index);
                }
                /* Do the "optimized Goertzel" processing. */

                //magnitude[i] = sqrt((lf_Vk_2 * lf_Vk_2) + (lf_Vk_1 * lf_Vk_1) - coeff[i] * lf_Vk_2 * lf_Vk_1 );
                magnitude[i] = sqrt((hf_Vk[_N+1] * hf_Vk[_N+1]) + (hf_Vk[_N] * hf_Vk[_N]) - coeff[i] * hf_Vk[_N] * hf_Vk[_N+1] );
                //magnitude[i] = sqrt(Q1*Q1 + Q2*Q2 - coeff[i]*Q1*Q2);
                ResetGoertzel();
            }


        };
};

#endif