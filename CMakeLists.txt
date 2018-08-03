cmake_minimum_required(VERSION 3.10)
project(Djiska)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV 3.3.0 REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})


add_executable(Djiska main.cpp)
target_link_libraries(Djiska ${OpenCV_LIBS})