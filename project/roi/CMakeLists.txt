cmake_minimum_required(VERSION 2.6)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(demo)

set(OpenCV_DIR "/usr/local/opencv-3.4.2/share/OpenCV")

set(INC_DIR 
         ${PROJECT_SOURCE_DIR}/inc
)
include_directories(${INC_DIR})
find_package(OpenCV REQUIRED)

add_executable(demo     main.cpp 
                        src/roi.cpp
            )


target_link_libraries(demo ${OpenCV_LIBS} )

