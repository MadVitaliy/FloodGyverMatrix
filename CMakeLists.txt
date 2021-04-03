cmake_minimum_required(VERSION 3.12)
project(MouseAndClipboard)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(MouseAndClipboard main.cpp)