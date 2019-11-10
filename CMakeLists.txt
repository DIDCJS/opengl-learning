cmake_minimum_required(VERSION 2.8)

SET(PROJ_NAME OPENGL_LEARN)

PROJECT(${PROJ_NAME})

SET(SRC_PATH ${PROJECT_SOURCE_DIR}/src)
SET(INC_PATH ${PROJECT_SOURCE_DIR}/include)
SET(3RD_PATH ${PROJECT_SOURCE_DIR}/3rdParty/3rd)

MESSAGE("###project_source_dir: " ${PROJECT_SOURCE_DIR})
MESSAGE("###src_path: " ${SRC_PATH})
MESSAGE("###3rd_path: " ${3RD_PATH})

include_directories(
    ${INC_PATH}
    ${3RD_PATH}/opengl-glfw-3.2.1/win32/include
    ${3RD_PATH}/glad/include
    ${3RD_PATH}/KHR
    ${3RD_PATH}/glm-master

)

file(GLOB_RECURSE FILE_CPP 
    ${SRC_PATH}/main.cpp
    ${SRC_PATH}/LearnGL.cpp
    ${SRC_PATH}/GLShaders.cpp
    ${SRC_PATH}/GLRenders.cpp
    ${3RD_PATH}/glad/src/glad.c
)

#引入外部库
LINK_DIRECTORIES(${3RD_PATH}/opengl-glfw-3.2.1/win32/libs)


ADD_EXECUTABLE(${PROJ_NAME}
	${FILE_CPP}
)

TARGET_LINK_LIBRARIES(${PROJ_NAME} 
        glfw3
        opengl32
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
