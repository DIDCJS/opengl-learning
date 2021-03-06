cmake_minimum_required(VERSION 2.8)

SET(PROJ_NAME OPENGL_LEARN)

PROJECT(${PROJ_NAME})

SET(SRC_PATH ${PROJECT_SOURCE_DIR}/src)
SET(INC_PATH ${PROJECT_SOURCE_DIR}/include)
SET(3RD_PATH ${PROJECT_SOURCE_DIR}/3rd)

message("###project_source_dir: " ${PROJECT_SOURCE_DIR})
message("###src_path: " ${SRC_PATH})
message("###3rd_path: " ${3RD_PATH})


SET(INC_FILES ${INC_PATH})
#source_group(include FILES ${INC_FILES})
#source_group(include REGULAR_EXPRESSION "${INC_FILES}/*.h")
file(GLOB_RECURSE Header_Files ${INC_FILES}/*.h)
source_group(includes FILES ${Header_Files})


link_directories(${3RD_PATH}/opengl-glfw-3.2.1/win32/libs)




include_directories(
    ${INC_PATH}
    ${3RD_PATH}/opengl-glfw-3.2.1/win32/include
    ${3RD_PATH}/glad/include
    ${3RD_PATH}/KHR
    ${3RD_PATH}/glm-master

)


add_executable(${PROJ_NAME}
    ${INC_FILES}
    ${Header_Files}
    ${SRC_PATH}/main.cpp
    ${SRC_PATH}/LearnGL.cpp
    ${SRC_PATH}/GLShaders.cpp
    ${SRC_PATH}/GLRenders.cpp
    ${3RD_PATH}/glad/src/glad.c


)



target_link_libraries(${PROJ_NAME} 
        glfw3
        opengl32
)