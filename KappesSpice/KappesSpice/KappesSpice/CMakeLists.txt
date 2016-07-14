cmake_minimum_required(VERSION 3.3)
project(HelloWorld)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Componente.cpp Componente.h LinearSystem.cpp LinearSystem.h netlist.cpp Netlist.h NetlistReadingProgram.cpp)
add_executable(NetlistReadingProgram ${SOURCE_FILES})