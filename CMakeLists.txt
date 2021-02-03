cmake_minimum_required(VERSION 3.0)

project(ads1256 LANGUAGES C)

add_executable(ads1256 main.c)

install(TARGETS ads1256 RUNTIME DESTINATION bin)
