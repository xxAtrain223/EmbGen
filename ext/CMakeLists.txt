cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(ext)

add_library(tinyxml2 tinyxml2/tinyxml2.cpp)
target_include_directories(tinyxml2 PUBLIC tinyxml2)

if (EMB_ENABLE_TESTING)
    add_subdirectory(googletest)
endif()
