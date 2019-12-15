cmake_minimum_required(VERSION 2.8)

SET(PROJ_NAME OPENGL_LEARN)

PROJECT(${PROJ_NAME})

SET(SRC_PATH ${PROJECT_SOURCE_DIR}/src)
SET(INC_PATH ${PROJECT_SOURCE_DIR}/include)
SET(3RD_PATH ${PROJECT_SOURCE_DIR}/3rdParty/3rd)
SET(SHADER_PATH ${PROJECT_SOURCE_DIR}/shaders)

MESSAGE("###project_source_dir: " ${PROJECT_SOURCE_DIR})
MESSAGE("###src_path: " ${SRC_PATH})
MESSAGE("###3rd_path: " ${3RD_PATH})

INCLUDE_DIRECTORIES(
    ${INC_PATH}
    ${3RD_PATH}/opengl-glfw-3.2.1/win32/include
    ${3RD_PATH}/glad/include
    ${3RD_PATH}/KHR
    ${3RD_PATH}/glm-master
	${3RD_PATH}/assimp/include

)



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
#分文件夹
SOURCE_GROUP(includes FILES ${Header_Files})
SOURCE_GROUP(src FILES ${FILE_CPP})
SOURCE_GROUP(shaders FILES ${FILE_SHADERS})

#引入外部库
LINK_DIRECTORIES(${3RD_PATH}/opengl-glfw-3.2.1/win32/libs)
LINK_DIRECTORIES(${3RD_PATH}/assimp/libs)

ADD_EXECUTABLE(${PROJ_NAME}
	${FILE_CPP}
	 ${Header_Files}
	 ${FILE_SHADERS}
)

TARGET_LINK_LIBRARIES(${PROJ_NAME} 
        glfw3
        opengl32
		assimp-vc140-mt
)


#OpenCV
FIND_PACKAGE(OpenCV REQUIRED)	

    
#如果找不到，可以手动设置
#set(OpenCV_DIR /opt/opencv-2.4.11/share/OpenCV)
IF(OpenCV_FOUND)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${PROJ_NAME} ${OpenCV_LIBS})
ELSE()
MESSAGE("### CANT FIND OPENCV")
ENDIF()
