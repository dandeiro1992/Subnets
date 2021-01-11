cmake_minimum_required(VERSION 3.5.1)

# project name
project(damian-net-database)

# creates the variable EXEC and sets it to main
set(EXEC main)

# set the C++17 standard
set(CMAKE_CXX_STANDARD 17)

#set additional flags
set(CMAKE_CXX_FLAGS "-O2")

# I../includes
include_directories(includes)

# puts all .cpp files inside src to the SOURCES variable
file(GLOB SOURCES src/*.cpp)

# compiles the files defined by SOURCES to generante the executable defined by EXEC
add_executable(${EXEC} ${SOURCES})
