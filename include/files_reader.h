#ifndef FILESREADER_H
#define FILESREADER_H

#include <vector>
#include "properties.h"

struct AudioFileInfo {
    std::string filePath;
    std::vector<float> data;
    int sampleRate;
    int channels;
};

class FilesReader {
public:
    explicit FilesReader(Properties* properties);
    void loadData();
    std::vector<AudioFileInfo> getData() const;
    std::vector<AudioFileInfo> getDataAt(int index) const;

private:
    Properties* properties;
    std::vector<AudioFileInfo> musicStorage;
};

#endif  // FILESREADER_H