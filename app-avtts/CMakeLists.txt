cmake_minimum_required(VERSION 3.0)

project(app-avtts)

set(EXECUTABLE_OUTPUT_PATH ../bin/${CMAKE_BUILD_TYPE})

include_directories(./include)
include_directories(${TOOLKIT_MODEL_INCLUDE_DIR})
include_directories(${TOOLKIT_QT_INCLUDE_DIR})
include_directories(${TOOLKIT_TTS_INCLUDE_DIR})

include_directories(${JNI_INCLUDE_DIRS})

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Gui_INCLUDE_DIRS})
include_directories(${Qt5OpenGL_INCLUDE_DIRS})


link_directories(${TOOLKIT_MODEL_LIBRARY})
link_directories(${TOOLKIT_QT_LIBRARY})
link_directories(${TOOLKIT_TTS_LIBRARY})

link_directories(${QT_LIB_DIR})

file(
	GLOB_RECURSE
	source_files
	src/*.cpp
	include/*.h
	ui/*
)
    
add_executable(
	app-avtts
	
	${source_files} 
)



target_link_libraries(

        app-avtts

	${TOOLKIT_TTS_LIBRARY}
	${TOOLKIT_QT_LIBRARY}
	${TOOLKIT_MODEL_LIBRARY}
	  
	Qt5::Widgets
	Qt5::Core
	Qt5::Gui
	Qt5::OpenGL
	
	${JNI_LIBRARIES}
	
	${MISC_OPENGL_LIBRARY}
	${MISC_GLU_LIBRARY}
	${MISC_WS_LIBRARY}
	${MISC_IPHLP_LIBRARY}
	${MISC_SETUP_LIBRARY}	
)


