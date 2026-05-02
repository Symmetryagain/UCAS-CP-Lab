cmake_minimum_required(VERSION 3.11)

project(CACT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_BUILD_TYPE Debug)

# Include paths
include_directories(deps/antlr4-runtime/)
include_directories(generated/grammar)
include_directories(include)

# Source files
file(GLOB COMPILER_SRC "src/*.cpp")
file(GLOB GENERATED_SRC "generated/grammar/*.cpp")
file(GLOB INTERP_SRC  "src/interpreter/*.cpp")

# ANTLR runtime library
file(GLOB_RECURSE ANTLR_LIB_FILES "deps/antlr4-runtime/*.cpp")

add_definitions(-g -Wall)
add_library(antlr STATIC ${ANTLR_LIB_FILES})

# Compiler target
add_executable(compiler ${COMPILER_SRC} ${GENERATED_SRC})
find_package(Threads REQUIRED)
target_link_libraries(compiler antlr Threads::Threads)

# Interpreter target
add_executable(interpreter ${INTERP_SRC})
target_link_libraries(interpreter Threads::Threads)
