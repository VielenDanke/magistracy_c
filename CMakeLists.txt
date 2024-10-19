cmake_minimum_required(VERSION 3.29)
project(untitled C)

set(CMAKE_C_STANDARD 11)

add_library(untitled STATIC library.c
        os_architecture/lab_1/task_1.c
        os_architecture/lab_1/task_2.c
        os_architecture/lab_1/task_4.c)
