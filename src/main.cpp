#include <iostream>
#include "../include/properties.h"
#include "../include/files_reader.h"

int main() {
    try {
        Properties props;
        std::cout << "Path: " << props.getPath() << std::endl;
        FilesReader filesReader{&props};
        filesReader.loadData();
        std::cout << "Download complete! Number of examples " << filesReader.getData().size();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
