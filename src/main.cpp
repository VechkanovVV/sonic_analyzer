#include <iostream>
#include "../include/audio_normalizer.h"
#include "../include/fft_analyzer.h"
#include "../include/files_reader.h"
#include "../include/properties.h"

int main() {
    try {
        Properties props;
        std::cout << "Path: " << props.getPath() << std::endl;
        FilesReader filesReader{&props};
        filesReader.loadData();
        std::cout << "Download complete! Number of examples " << filesReader.getData().size() << std::endl;
        AudioNormalizer audioNormalizer{&filesReader};
        audioNormalizer.normalizeAudio();
        FFTAnalyzer fftAnalyzer{};
        for (const auto& audioFile : filesReader.getData()) {
            AnalysisResult audioResult = fftAnalyzer.analyze(audioFile);
            std::cout << "The composition with path: " << audioFile.filePath << " has been analyzed." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
