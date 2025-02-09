#include <cufft.h>

extern "C" {
    void executeFFT(float* d_input, cufftComplex* d_output, int signalSize) {
        cufftHandle plan;
        cufftPlan1d(&plan, signalSize, CUFFT_R2C, 1);
        cufftExecR2C(plan, d_input, d_output);
        cufftDestroy(plan);
    }
}
