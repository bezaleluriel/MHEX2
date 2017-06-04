cmake_minimum_required(VERSION 3.6)
project(MHEX2LIRON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

set(SOURCE_FILES ex2_upd.c)
add_executable(MHEX2LIRON ${SOURCE_FILES})