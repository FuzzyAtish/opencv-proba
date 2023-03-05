cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)
project(ip-labs-vscode-win VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenCV REQUIRED)

add_definitions(-DASSETS_DIR="${CMAKE_SOURCE_DIR}/assets/")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/include/util/nfde")

add_executable(${PROJECT_NAME}
        src/util/file-util.cpp
        src/util/image-util.cpp
        src/labs/lab.cpp
        src/labs/lab1.cpp
        src/labs/lab2.cpp
        src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include/labs ${CMAKE_CURRENT_LIST_DIR}/include/util)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} nfd)
