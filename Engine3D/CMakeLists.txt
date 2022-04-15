cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(EngineEXE)

set(OpenGL_GL_PREFERENCE LEGACY)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Basta comentar uma das linhas 'add_executable' e tirar a outra de comentário para alterar entre os executaveis
add_executable(${PROJECT_NAME} Engine/Engine.cpp Generator/BoxGenerator.cpp Generator/BoxGenerator.h Generator/ConeGenerator.cpp Generator/ConeGenerator.h Generator/BenzierGenerator Generator/PlaneGenerator.cpp Generator/PlaneGenerator.h Generator/SphereGenerator.cpp Generator/TorusGenerator.cpp Generator/SphereGenerator.h Generator/TorusGenerator.h Generator/BenzierGenerator.cpp XMLReader/xmlReader.cpp XMLReader/xmlReader.hpp Auxiliar/AuxiliarMethods.cpp Auxiliar/AuxiliarMethods.h Auxiliar/Point.h Generator/Model.cpp Generator/Model.h)
add_executable(GeneratorEXE  Generator/BoxGenerator.cpp Generator/BoxGenerator.h Generator/ConeGenerator.cpp Generator/ConeGenerator.h Generator/BenzierGenerator.h Generator/Generator.cpp Generator/PlaneGenerator.cpp Generator/PlaneGenerator.h Generator/SphereGenerator.cpp Generator/TorusGenerator.cpp Generator/SphereGenerator.h Generator/TorusGenerator.h Generator/BenzierGenerator.cpp Auxiliar/AuxiliarMethods.cpp Auxiliar/AuxiliarMethods.h Generator/Model.cpp Generator/Model.h)


find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
	set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
	
	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")	
	
	if (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")
		message(ERROR ": GLEW not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")


	include_directories(${TOOLKITS_FOLDER}/glut ${TOOLKITS_FOLDER}/glew)
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES}
										  ${TOOLKITS_FOLDER}/glut/glut32.lib
										  ${TOOLKITS_FOLDER}/glew/glew32.lib )

	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll"  AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll")
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${TOOLKITS_FOLDER}/glew/glew32.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll"  AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll")

	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

else (WIN32) #Linux and Mac

	set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")

	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIR})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})

	if (NOT APPLE)
		find_package(GLEW REQUIRED)
		include_directories(${GLEW_INCLUDE_DIRS})
		link_libraries(${GLEW_LIBRARIES})
		target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
	else (NOT APPLE)
		target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
	endif(NOT APPLE)

	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
	if(NOT GLUT_FOUND)
	   message(ERROR ": GLUT not found!")
	endif(NOT GLUT_FOUND)

endif(WIN32)









