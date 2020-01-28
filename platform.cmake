
# link lib
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

	target_link_libraries(${PROJ_NAME} "-framework OpenGL")
	target_link_libraries(${PROJ_NAME} "-framework GameKit")
	target_link_libraries(${PROJ_NAME} "-framework CoreVideo")
	target_link_libraries(${PROJ_NAME} "-framework IOKit")
	target_link_libraries(${PROJ_NAME} "-framework Accelerate")
ELSE()

ENDIF()

