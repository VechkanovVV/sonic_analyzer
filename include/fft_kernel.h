#pragma once
#include <cufft.h>

#ifdef __cplusplus
extern "C" {
#endif

void executeFFT(float* d_input, cufftComplex* d_output, int signalSize);

#ifdef __cplusplus
}
#endif
