#ifndef goertzel_hpp
#define goertzel_hpp


#include "mbed.h"

#define _samplingFrequency 8000
#define _N 200
#define PI 3.14

class goertzel {


    private:
        double freq_centres[8] = {697,770,852,941,1209,1336,1477,1633};

        double coeff[8];
        double Q1[8];
        double Q2[8];

        void ProcessSample(int sample){
            float Q0[8];
            for (int i = 0;i<8;i++){
                Q0[i] = coeff[i] * Q1[i] - Q2[i] + float(sample);
                Q2[i] = Q1[i];
                Q1[i] = Q0[i];
            }
        }
	    void ResetGoertzel(){
            for (int i = 0;i<8;i++){
                Q2[i] = 0;
                Q1[i] = 0;
            }
        }
    public:

        goertzel() {
            double omega[8];
            for (int i = 0;i<8;i++){
                omega[i] = (2.0 * PI * freq_centres[i] ) / _samplingFrequency;
                coeff[i] = 2.0 * cos(omega[i]);
            };
            ResetGoertzel();

        }

	    double* detect(int testData[_N]) {
            double    *magnitude[8];

            /* Process the samples. */
            for (int index = 0; index < _N; index++)
            {
                ProcessSample(testData[index]);
            }

            /* Do the "optimized Goertzel" processing. */
            for (int i = 0;i<8;i++){
                *magnitude[i] = sqrt(Q1[i]*Q1[i] + Q2[i]*Q2[i] - coeff[i]*Q1[i]*Q2[i]);
            };
            ResetGoertzel();
            return *magnitude;

        };
        

};

#endif