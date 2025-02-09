#include "../include/audio_normalizer.h"
#include <algorithm>
#include <iostream>

const int WINDOW_SIZE = 1024;

AudioNormalizer::AudioNormalizer(FilesReader* filesReader) : filesReader(filesReader){};

void AudioNormalizer::normalizeAudio() {
    for (auto& audioFileInfo : filesReader->getData()) {
        float maxSignalValue = 0;
        for (const auto& signalValue : audioFileInfo.data) {
            maxSignalValue = std::max(maxSignalValue, signalValue);
        }
        for (auto& signal : audioFileInfo.data) {
            signal /= maxSignalValue;
        }
    }
}