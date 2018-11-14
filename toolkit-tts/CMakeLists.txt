cmake_minimum_required(VERSION 3.0)


##### toolkit-marytts [JAVA]########

set(JAR_NAME toolkit-marytts)
set(JAVA_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(
	JAVA_SOURCE_FILES 
	${JAVA_SOURCE_DIRECTORY}/MaryTTSClient.java
)

add_jar(
	${JAR_NAME}
	SOURCES
	${JAVA_SOURCE_FILES}
	#OUTPUT_DIR ../lib/${CMAKE_BUILD_TYPE}
)

get_target_property(_jarFile toolkit-marytts JAR_FILE)
get_target_property(_classDir toolkit-marytts CLASSDIR)

set(JAVA_CLASS_DIRECTORY "${_classDir}/application")
set(DESTINATION_DEBUG "../bin/Debug/")
set(DESTINATION_RELEASE "../bin/Release/")

add_custom_command(
 TARGET toolkit-marytts POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E copy_directory ${JAVA_CLASS_DIRECTORY} ${DESTINATION_DEBUG} 
)


add_custom_command(
 TARGET toolkit-marytts POST_BUILD
 COMMAND ${CMAKE_COMMAND} -E copy_directory ${JAVA_CLASS_DIRECTORY} ${DESTINATION_RELEASE} 
)


##### toolkit-tts [C++]########
project(toolkit-tts)

set(LIBRARY_OUTPUT_PATH ../lib/${CMAKE_BUILD_TYPE})

include_directories(./include)

include_directories(${JNI_INCLUDE_DIRS})

file(
	GLOB_RECURSE
	source_files
	src/*.cpp
	include/*.h
)
    
add_library(
	toolkit-tts
	STATIC
	${source_files} 
)