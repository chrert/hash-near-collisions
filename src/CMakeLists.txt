cmake_minimum_required(VERSION 2.8.4)
project(collider)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm -std=c11 -march=native -Wall -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS}  -O0 -g -fstack-protector-all")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS}  -O3")

set(CMAKE_VERBOSE_MAKEFILE off)

set(SOURCE_FILES
    main.c
    collision.c
    bitutils.c
    hamming_code.c)

add_executable(collider ${SOURCE_FILES})

add_subdirectory(md5)
target_link_libraries(collider md5_lib)
