cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(EngineEXE)

set(OpenGL_GL_PREFERENCE LEGACY)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Basta comentar uma das linhas 'add_executable' e tirar a outra de comentário para alterar entre os executaveis
add_executable(${PROJECT_NAME} Engine/Engine.cpp Generator/BoxGenerator.cpp Generator/BoxGenerator.h Generator/ConeGenerator.cpp Generator/ConeGenerator.h Generator/PlaneGenerator.cpp Generator/PlaneGenerator.h Generator/SphereGenerator.cpp Generator/TorusGenerator.cpp Generator/SphereGenerator.h Generator/TorusGenerator.h XMLReader/xmlReader.cpp XMLReader/xmlReader.hpp Auxiliar/AuxiliarMethods.cpp Auxiliar/AuxiliarMethods.h Engine/ModelInfo.cpp Engine/ModelInfo.h Generator/Generator.h)
add_executable(GeneratorEXE  Generator/BoxGenerator.cpp Generator/BoxGenerator.h Generator/ConeGenerator.cpp Generator/ConeGenerator.h Generator/Generator.cpp Generator/PlaneGenerator.cpp Generator/PlaneGenerator.h Generator/SphereGenerator.cpp Generator/TorusGenerator.cpp Generator/SphereGenerator.h Generator/TorusGenerator.h XMLReader/xmlReader.cpp XMLReader/xmlReader.hpp Auxiliar/AuxiliarMethods.cpp Auxiliar/AuxiliarMethods.h Generator/Generator.h)


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
	
	
	include_directories(${TOOLKITS_FOLDER}/glut )
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} 
										  ${TOOLKITS_FOLDER}/glut/glut32.lib)
	target_link_libraries(GeneratorEXE ${OPENGL_LIBRARIES} 
										  ${TOOLKITS_FOLDER}/glut/glut32.lib)
	
	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )	
	
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	
else (WIN32) #Linux and Mac

	set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")
	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIR})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})
	
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
	target_link_libraries(GeneratorEXE ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
	if(NOT GLUT_FOUND)
	   message(ERROR ": GLUT not found!")
	endif(NOT GLUT_FOUND)
	
endif(WIN32)








