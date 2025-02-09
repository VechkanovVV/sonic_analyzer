#include "../include/fft_analyzer.h"
#include <cuda_runtime.h>
#include <cufft.h>
#include <complex>
#include <iostream>
#include <vector>
#include "../include/fft_kernel.h"
#include "../include/files_reader.h"

std::pair<std::vector<float>, std::vector<float>>
FFTAnalyzer::splitChannels(const std::vector<float>& audioData, int channels) {
    std::vector<float> left, right;
    size_t totalSamples = audioData.size();

    for (size_t i = 0; i < totalSamples; i += channels) {
        left.push_back(audioData[i]);
        if (channels >= 2 && (i + 1) < totalSamples) {
            right.push_back(audioData[i + 1]);
        }
    }
    return std::make_pair(left, right);
}

std::vector<float> FFTAnalyzer::computeAmplitudeSpectrum(const std::vector<std::complex<float>>& fftResult) {
    std::vector<float> amplitudes;
    amplitudes.reserve(fftResult.size());
    for (const auto& bin : fftResult) {
        amplitudes.push_back(std::abs(bin));
    }
    return amplitudes;
}

std::vector<float> FFTAnalyzer::processChannel(const std::vector<float>& channel, int sampleRate) {
    float* d_input = nullptr;
    cufftComplex* d_fftResult = nullptr;
    size_t channelSize = channel.size();

    cudaError_t err = cudaMalloc(&d_input, channelSize * sizeof(float));
    if (err != cudaSuccess) {
        std::cerr << "cudaMalloc failed for d_input: " << cudaGetErrorString(err) << "\n";
        return {};
    }

    size_t fftSize = channelSize / 2 + 1;
    err = cudaMalloc(&d_fftResult, fftSize * sizeof(cufftComplex));
    if (err != cudaSuccess) {
        std::cerr << "cudaMalloc failed for d_fftResult: " << cudaGetErrorString(err) << "\n";
        cudaFree(d_input);
        return {};
    }

    err = cudaMemcpy(d_input, channel.data(), channelSize * sizeof(float), cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        std::cerr << "cudaMemcpy (Host to Device) failed: " << cudaGetErrorString(err) << "\n";
        cudaFree(d_input);
        cudaFree(d_fftResult);
        return {};
    }

    executeFFT(d_input, d_fftResult, static_cast<int>(channelSize));

    std::vector<cufftComplex> fftResult(fftSize);
    err = cudaMemcpy(fftResult.data(), d_fftResult, fftSize * sizeof(cufftComplex), cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {
        std::cerr << "cudaMemcpy (Device to Host) failed: " << cudaGetErrorString(err) << "\n";
        cudaFree(d_input);
        cudaFree(d_fftResult);
        return {};
    }

    std::vector<std::complex<float>> fftResultComplex;
    fftResultComplex.reserve(fftResult.size());
    for (const auto& bin : fftResult) {
        fftResultComplex.emplace_back(bin.x, bin.y);
    }

    std::vector<float> amplitudes = computeAmplitudeSpectrum(fftResultComplex);

    cudaFree(d_input);
    cudaFree(d_fftResult);

    return amplitudes;
}

AnalysisResult FFTAnalyzer::analyze(const AudioFileInfo& audioFile) {
    AnalysisResult result;
    result.filePath = audioFile.filePath;

    auto channelsPair = splitChannels(audioFile.data, audioFile.channels);

    if (!channelsPair.first.empty()) {
        result.leftChannelAmplitudes = processChannel(channelsPair.first, audioFile.sampleRate);
    }
    if (!channelsPair.second.empty()) {
        result.rightChannelAmplitudes = processChannel(channelsPair.second, audioFile.sampleRate);
    }

    return result;
}
