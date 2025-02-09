#ifndef AUDIONORMALIZER_H
#define AUDIONORMALIZER_H

#include "files_reader.h"

class AudioNormalizer {
public:
    explicit AudioNormalizer(FilesReader* filesReader);

    void normalizeAudio();

private:
    FilesReader* filesReader;
};
#endif  // AUDIONORMALIZER_H