cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)
project(ip-labs-vscode-win VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
