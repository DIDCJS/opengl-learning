cmake_minimum_required(VERSION 3.13)
# set the name of building project
SET(PROJ_NAME OPENGL_LEARN)
PROJECT(${PROJ_NAME})

set(CMAKE_CXX_STANDARD 11)
# set the global variable
SET(SRC_PATH ${PROJECT_SOURCE_DIR}/src)
SET(INC_PATH ${PROJECT_SOURCE_DIR}/include)
SET(3RD_PATH ${PROJECT_SOURCE_DIR}/3rdParty)
SET(SHADER_PATH ${PROJECT_SOURCE_DIR}/shaders)

# LOG TEST
MESSAGE("###project_source_dir: " ${PROJECT_SOURCE_DIR})
MESSAGE("###src_path: " ${SRC_PATH})
MESSAGE("###3rd_path: " ${3RD_PATH})
MESSAGE("###system: " ${CMAKE_SYSTEM_NAME})

INCLUDE("${3RD_PATH}/3rd.cmake")


FILE(GLOB_RECURSE Header_Files 
	${INC_PATH}/*.h
)

File(GLOB_RECURSE FILE_CPP 
    ${SRC_PATH}/*.cpp
    ${3RD_PATH}/glad/src/glad.c
)

File(GLOB_RECURSE FILE_SHADERS 
     ${SHADER_PATH}/*
)

# in order to show the catalog in vs IDE
SOURCE_GROUP(includes FILES ${Header_Files})
SOURCE_GROUP(src FILES ${FILE_CPP})
SOURCE_GROUP(shaders FILES ${FILE_SHADERS})

ADD_EXECUTABLE(${PROJ_NAME}
	${FILE_CPP}
	 ${Header_Files}
	 ${FILE_SHADERS}
)

#�����ⲿ��
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	LINK_DIRECTORIES(${3RD_PATH}/opengl-glfw-3.2.1/win32/libs)
	LINK_DIRECTORIES(${3RD_PATH}/assimp/libs)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	MESSAGE("### step1 MAC")
	#	LINK_DIRECTORIES(${3RD_PATH}/glfw-3.3.2.bin.MACOS/lib-macos)
	#	LINK_DIRECTORIES("/Users/jsc/project-own/opengl-learning/3rdParty/glfw-3.3.2.bin.MACOS/lib-macos")
	LINK_DIRECTORIES(${3RD_PATH}/assimp/assimp-5.0.1/build/lib)
ELSE()
	LINK_DIRECTORIES(${3RD_PATH}/opengl-glfw-3.2.1/win32/libs)
	LINK_DIRECTORIES(${3RD_PATH}/assimp/libs)
ENDIF()



IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	TARGET_LINK_LIBRARIES(${PROJ_NAME}
			glfw3
			opengl32
			assimp-vc140-mt
			)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	TARGET_LINK_LIBRARIES(${PROJ_NAME}
#            libglfw3.a
			/usr/local/lib/libglfw3.a
#			opengl32
#			libassimp.dylib
			${3RD_PATH}/assimp/assimp-5.0.1/build/lib/libassimp.dylib
			)
	target_link_libraries(${PROJ_NAME} "-framework OpenGL")
	target_link_libraries(${PROJ_NAME} "-framework GameKit")
	target_link_libraries(${PROJ_NAME} "-framework CoreVideo")
	target_link_libraries(${PROJ_NAME} "-framework IOKit")
	target_link_libraries(${PROJ_NAME} "-framework Accelerate")
ELSE()
	TARGET_LINK_LIBRARIES(${PROJ_NAME}
			glfw3
			opengl32
			assimp-vc140-mt
			)
ENDIF()



#OpenCV
FIND_PACKAGE(OpenCV 3. REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")



#����Ҳ����������ֶ�����
#set(OpenCV_DIR /opt/opencv-2.4.11/share/OpenCV)
IF(OpenCV_FOUND)
message("find opencv")
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${PROJ_NAME} ${OpenCV_LIBS})
ELSE()
MESSAGE("### CANT FIND OPENCV")
ENDIF()
