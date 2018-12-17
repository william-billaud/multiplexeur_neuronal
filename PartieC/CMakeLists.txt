cmake_minimum_required(VERSION 3.12)
project(RNA C)

set(CMAKE_C_STANDARD 99)

add_executable(RNA main.c Data.c Data.h neurones.c neurones.h menu.c menu.h)
target_link_libraries(RNA -lm)