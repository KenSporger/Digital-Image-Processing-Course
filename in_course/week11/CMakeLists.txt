#制定cmake版本
cmake_minimum_required(VERSION 2.6)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(demo)

set(INC_DIR 
         ${PROJECT_SOURCE_DIR}/inc
)
include_directories(${INC_DIR})
find_package(OpenCV REQUIRED)

#生成可执行文件demo_exec 
#由于没有使用动态库，所以必须包含所有cpp文件
add_executable(demo     main.cpp 
                        src/functions.cpp
                        src/stdafx.cpp
            )


target_link_libraries(demo ${OpenCV_LIBS} )

