cmake_minimum_required(VERSION 3.10)

# Project name and version
project(FlowerExchange VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set default build type to Release for performance optimization if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags for warnings and aggressive optimization
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Define the include directory so the compiler can find our header files
include_directories(${CMAKE_SOURCE_DIR}/include)

# Gather all source files
set(SOURCES
    src/main.cpp
    # src/Validator.cpp
    # src/OrderBook.cpp
    # src/ExchangeApp.cpp
    # src/CSVHandler.cpp
)

# Create the executable
add_executable(flower_exchange ${SOURCES})
