cmake_minimum_required(VERSION 3.15)
project(yulskers)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(yulskers)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()