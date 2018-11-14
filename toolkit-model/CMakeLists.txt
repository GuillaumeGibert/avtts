cmake_minimum_required(VERSION 3.0)

project(toolkit-model)

set(LIBRARY_OUTPUT_PATH ../lib/${CMAKE_BUILD_TYPE})

include_directories(./include)

file(
	GLOB_RECURSE
	source_files
	src/*.cpp
	include/*.h
)
    
add_library(
	toolkit-model
	STATIC
	${source_files} 
)