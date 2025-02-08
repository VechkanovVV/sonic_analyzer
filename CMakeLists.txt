cmake_minimum_required(VERSION 3.20)
project(sonic_analyzer)

enable_language(CUDA)

set(CMAKE_CUDA_ARCHITECTURES 75)

find_package(PkgConfig REQUIRED)
find_package(nlohmann_json REQUIRED)
pkg_check_modules(SNDFILE REQUIRED sndfile)

include_directories(${SNDFILE_INCLUDE_DIRS})

add_executable(sonic_analyzer
    src/main.cpp
    src/properties.cpp
    src/files_reader.cpp
)

target_link_libraries(sonic_analyzer PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(sonic_analyzer PRIVATE ${SNDFILE_LIBRARIES})
target_compile_features(sonic_analyzer PRIVATE cuda_std_17)