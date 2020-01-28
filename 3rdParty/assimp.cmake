
# include 3rd
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	INCLUDE_DIRECTORIES(${3RD_PATH}/assimp/include)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	INCLUDE_DIRECTORIES(${3RD_PATH}/assimp/assimp-5.0.1/build/include)
ELSE()
	INCLUDE_DIRECTORIES(${3RD_PATH}/assimp/include)
ENDIF()

# link lib path
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	LINK_DIRECTORIES(${3RD_PATH}/assimp/libs)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	LINK_DIRECTORIES(${3RD_PATH}/assimp/assimp-5.0.1/build/lib)
ELSE()
	LINK_DIRECTORIES(${3RD_PATH}/assimp/libs)
ENDIF()


# link lib
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	TARGET_LINK_LIBRARIES(${PROJ_NAME} assimp-vc140-mt)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	TARGET_LINK_LIBRARIES(${PROJ_NAME} ${3RD_PATH}/assimp/assimp-5.0.1/build/lib/libassimp.dylib)
ELSE()
	TARGET_LINK_LIBRARIES(${PROJ_NAME} assimp-vc140-mt)
ENDIF()

