cmake_minimum_required(VERSION 3.10)

project(hello)

set(TARGET_SRCS
    "src/main.c"
)

set(TARGET_INCLUDES
    "include"
)

set(TARGET_FLAGS_HARDWARE "-m4")

set(CMAKE_C_FLAGS_DEBUG          "-DDEBUG -g -O0")
set(CMAKE_CXX_FLAGS_DEBUG        "-DDEBUG -g -O0")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "")

set(CMAKE_C_FLAGS_RELEASE          "-DNDEBUG -O2 -flto")
set(CMAKE_CXX_FLAGS_RELEASE        "-DNDEBUG -O2 -flto")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")

set(CMAKE_C_FLAGS          "${TARGET_FLAGS_HARDWARE} -Wall")
set(CMAKE_CXX_FLAGS        "${TARGET_FLAGS_HARDWARE} -Wall")
set(CMAKE_EXE_LINKER_FLAGS "${TARGET_FLAGS_HARDWARE} -Wall")
