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

# include inc
INCLUDE_DIRECTORIES(${INC_PATH})

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

# link 3rd
INCLUDE("${PROJECT_SOURCE_DIR}/3rd.cmake")

# link paltform
INCLUDE("${PROJECT_SOURCE_DIR}/platform.cmake")


