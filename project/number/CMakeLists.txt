cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(demo)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_module)

project(demo)

set(OpenCV_DIR "/usr/local/opencv-4.0.1/share/OpenCV")

set(LIB_DIRS /usr/lib/x86_64-linux-gnu)
set(INC_DIR 
         ${PROJECT_SOURCE_DIR}/inc

)

include_directories(${INC_DIR})
link_directories(${LIB_DIRS})
find_package(OpenCV REQUIRED)

add_executable(demo     main.cpp 
            )


target_link_libraries(demo ${OpenCV_LIBS} )

target_link_libraries(demo ${LIB_DIRS}   libtesseract.so
                                        liblept.so
                                            
)

