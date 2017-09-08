cmake_minimum_required(VERSION 3.8)
project(MAC0422___Sistemas_Operacionais)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        ep1/date.c
        ep1/date.h
        ep1/ep1.c
        ep1/ep1sh.c
        ep1/ls.c
        ep1/ls.h
        ep1/process.c
        ep1/process.h)

add_executable(MAC0422___Sistemas_Operacionais ${SOURCE_FILES})