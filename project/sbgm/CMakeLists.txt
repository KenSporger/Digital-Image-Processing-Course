cmake_minimum_required(VERSION 2.6)
SET(CMAKE_BUILD_TYPE "Debug")
#加入-pg参数使用gprof，需要编译两遍才会出gmount.out
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -pg  -Wall -g2 -ggdb ")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS}  -Wall -g2 -ggdb ")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
# SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -pg")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


project(demo)

set(OpenCV_DIR "/usr/local/opencv-3.4.2/share/OpenCV")
# set(OpenCV_DIR "/usr/local/opencv-4.0.1/lib/cmake/opencv4/")

# set(INC_DIR 
#          ${PROJECT_SOURCE_DIR}/inc
# )
# include_directories(${INC_DIR})
find_package(OpenCV REQUIRED)

add_executable(demo     main.cpp 
            )


target_link_libraries(demo ${OpenCV_LIBS} )

