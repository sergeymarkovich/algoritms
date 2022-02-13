cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(graphs)


add_executable(graphs main.cpp ListGraph.cpp MatrixGraph.cpp ArcGraph.cpp SetGraph.cpp)