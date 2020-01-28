
# include 3rd
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	INCLUDE_DIRECTORIES(${3RD_PATH}/opengl-glfw-3.2.1/win32/include)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#	INCLUDE_DIRECTORIES(/Users/jsc/project-own/opengl-learning/3rdParty/glfw/include)
	INCLUDE_DIRECTORIES(${3RD_PATH}/glfw/include)
ELSE()
	INCLUDE_DIRECTORIES(${3RD_PATH}/opengl-glfw-3.2.1/win32/include)
ENDIF()

# link lib path
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	LINK_DIRECTORIES(${3RD_PATH}/opengl-glfw-3.2.1/win32/libs)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("glfw step 2 ")
		message("glfw path : {${3RD_PATH}/glfw/lib}")
	LINK_DIRECTORIES(${3RD_PATH}/glfw/lib)
ELSE()
	LINK_DIRECTORIES(${3RD_PATH}/opengl-glfw-3.2.1/win32/libs)
ENDIF()


# link lib
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	TARGET_LINK_LIBRARIES(${PROJ_NAME} glfw3)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#	TARGET_LINK_LIBRARIES(${PROJ_NAME} /Users/jsc/project-own/opengl-learning/3rdParty/glfw/lib/libglfw3.a)
	TARGET_LINK_LIBRARIES(${PROJ_NAME} ${3RD_PATH}/glfw/lib/libglfw3.a)
ELSE()
	TARGET_LINK_LIBRARIES(${PROJ_NAME} glfw3)
ENDIF()
