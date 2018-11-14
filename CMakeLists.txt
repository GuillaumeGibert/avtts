cmake_minimum_required(VERSION 3.0)

#####CMAKE#####
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/../dist" CACHE FILEPATH "path to dist directory")

project(avtts)

####################
# 3rd parties libraries
####################

#####AUTO######

#JDK
set(JAVA_HOME "C:/Program Files/Java/jdk1.8.0_161")
find_package(Java REQUIRED COMPONENTS  Development)
find_package(JNI REQUIRED)
include(UseJava)

#QT
set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.8.0/5.8/msvc2015_64"  CACHE FILEPATH "path to qt5 vs2015 directory")
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Core)
find_package(Qt5OpenGL)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ./ui)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#####MANUAL######

# MISC
set(MISC_OPENGL_LIBRARY optimized opengl32 debug opengl32)
set(MISC_GLU_LIBRARY optimized glu32 debug glu32)
set(MISC_WS_LIBRARY optimized ws2_32 debug ws2_32)
set(MISC_IPHLP_LIBRARY optimized iphlpapi debug iphlpapi)
set(MISC_SETUP_LIBRARY optimized setupapi debug setupapi)

####################
# project libraries
####################

# include
set(TOOLKIT_MODEL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/toolkit-model/include" CACHE FILEPATH "path to toolkit-model include directory")
set(TOOLKIT_QT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/toolkit-qt/include" CACHE FILEPATH "path to toolkit-qt include directory")
set(TOOLKIT_TTS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/toolkit-tts/include" CACHE FILEPATH "path to toolkit-tts include directory")

# lib
set(TOOLKIT_MODEL_LIBRARY optimized toolkit-model debug toolkit-model)
set(TOOLKIT_QT_LIBRARY optimized toolkit-qt debug toolkit-qt)
set(TOOLKIT_TTS_LIBRARY optimized toolkit-tts debug toolkit-tts)


####################
# add project subdirectories
####################

# toolkit
add_subdirectory(toolkit-model)
add_subdirectory(toolkit-qt)
add_subdirectory(toolkit-tts)

# app
add_subdirectory(app-avtts)

####################
# INSTALL
####################

# bin, lib
install(DIRECTORY ${CMAKE_BINARY_DIR}/lib DESTINATION .)
install(DIRECTORY ${CMAKE_BINARY_DIR}/bin DESTINATION .)

# include
install(DIRECTORY ${TOOLKIT_MODEL_INCLUDE_DIR} DESTINATION .)
install(DIRECTORY ${TOOLKIT_QT_INCLUDE_DIR} DESTINATION .)
install(DIRECTORY ${TOOLKIT_TTS_INCLUDE_DIR} DESTINATION .)

####################
# set startup project
####################
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT app-avtts)
set_target_properties(app-avtts PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../dist/bin/$(Configuration)")
