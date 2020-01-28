

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
