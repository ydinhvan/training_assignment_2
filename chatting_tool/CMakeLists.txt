cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(ChattingTool VERSION 1.0)

include_directories(include)

add_subdirectory(source)

#add_subdirectory(source_cpp)

add_subdirectory(unit_test)