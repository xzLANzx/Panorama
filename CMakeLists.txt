cmake_minimum_required(VERSION 3.10)
project(panorama)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

set(HEADER_FILES )
set(SOURCE_FILES main.cpp ransac.cpp match.cpp harrisCornerDetection.cpp stitch.cpp functions.cpp panorama.cpp)

add_executable(panorama ${SOURCE_FILES})

include_directories(panorama ${OpenCV_INCLUDE_DIRS})

target_link_libraries(panorama ${OpenCV_LIBS})

#set(OPENCV_ENABLE_NONFREE "Enable non-free algorithms" ON)

#if(OPENCV_ENABLE_NONFREE)
#    message("  Non-free algorithms are enabled")
#else()
#    message("  Non-free algorithms are NOT enabled")
#endif()