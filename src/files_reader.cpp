#include "../include/files_reader.h"
#include <sndfile.h>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

FilesReader::FilesReader(Properties* properties) {
    this->properties = properties;
}

void FilesReader::loadData(){
    const std::string folderPath = properties->getPath();
    
    if (!fs::exists(folderPath)) {
        throw std::runtime_error("Folder does not exists: " + folderPath);
    }

    for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".wav") {
            SF_INFO sfInfo;
            SNDFILE* audioFile = sf_open(entry.path().c_str(), SFM_READ, &sfInfo);
            if (!audioFile) {
                std::cerr << "File opening error: " << sf_strerror(audioFile) << std::endl;
                continue;
            }

            std::vector<float> buffer(sfInfo.frames * sfInfo.channels);
            sf_count_t framesRead = sf_readf_float(audioFile, buffer.data(), sfInfo.frames);
            if (framesRead != sfInfo.frames) {
                std::cerr << "Data reading error: " << sf_strerror(audioFile) << std::endl;
                sf_close(audioFile);
                continue;
            }


            AudioFileInfo audioFileInfo;
            audioFileInfo.filePath = entry.path().string();
            audioFileInfo.data = std::move(buffer);
            audioFileInfo.sampleRate = sfInfo.samplerate;
            audioFileInfo.channels = sfInfo.channels;

            musicStorage.push_back(std::move(audioFileInfo));
            sf_close(audioFile);
        }
    }
}

std::vector<AudioFileInfo> FilesReader::getData() const{
    return musicStorage;
}