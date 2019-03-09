cmake_minimum_required(VERSION 3.10)
project(panorama)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

set(HEADER_FILES harrisCorner.h)
set(SOURCE_FILES main.cpp harrisCorner.cpp)

add_executable(panorama ${SOURCE_FILES})

include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(panorama ${OpenCV_LIBS})