cmake_minimum_required(VERSION 3.20)
project(sonic_analyzer)

enable_language(CUDA)

set(CMAKE_CUDA_ARCHITECTURES 75)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SNDFILE REQUIRED sndfile)
find_package(nlohmann_json REQUIRED) 
find_package(CUDAToolkit REQUIRED)

add_executable(sonic_analyzer
    src/main.cpp
    src/properties.cpp
    src/files_reader.cpp
    src/audio_normalizer.cpp
    src/fft_analyzer.cpp
    cuda/fft_kernel.cu 
)

target_include_directories(sonic_analyzer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(sonic_analyzer PRIVATE 
    ${SNDFILE_LIBRARIES}
    CUDA::cufft
    CUDA::cudart
    nlohmann_json::nlohmann_json
)

target_compile_features(sonic_analyzer PRIVATE cuda_std_17)
