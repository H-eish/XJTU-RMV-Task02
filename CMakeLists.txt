cmake_minimum_required(VERSION 3.10.0)
project(opencv_project VERSION 0.1.0 LANGUAGES C CXX)

add_executable(main src/main.cpp)
add_executable(task2 src/task2.cpp)

find_package(OpenCV REQUIRED)
include_directories(
    ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(main
    ${OpenCV_LIBS}
)

target_link_libraries(task2
    ${OpenCV_LIBS}
)