#pragma once
#include <cufft.h>
#include <complex>
#include <vector>
#include "../include/files_reader.h"

struct AnalysisResult {
    std::string filePath;
    std::vector<float> leftChannelAmplitudes;
    std::vector<float> rightChannelAmplitudes;
};

class FFTAnalyzer {
public:
    FFTAnalyzer() = default;
    AnalysisResult analyze(const AudioFileInfo& audioFile);

private:
    std::pair<std::vector<float>, std::vector<float>> splitChannels(const std::vector<float>& audioData, int channels);
    std::vector<float> processChannel(const std::vector<float>& channel, int sampleRate);
    std::vector<float> computeAmplitudeSpectrum(const std::vector<std::complex<float>>& fftResult);
};