cmake_minimum_required(VERSION 2.8.4)
project(collider)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -march=native -Wall -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS}  -O0 -g -fstack-protector-all")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS}  -O3")

set(CMAKE_VERBOSE_MAKEFILE off)

set(SOURCE_FILES
    main.c
    hash_function.c
    brents.c
    distinguished_points.c
    bitutils.c
    hamming_code.c
    dp_hash_table.c)

add_definitions(-DHASH_CONFIG=${HASH_CONFIG})

add_executable(collider ${SOURCE_FILES})

add_subdirectory(md5)

target_link_libraries(collider -lm -pthread md5_lib)
