cmake_minimum_required(VERSION 3.10)

project(infinite-curve)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 REQUIRED graphics window system)

set (${PROJECT_NAME}_headers
	src/Curve.h
	src/Character.h
	src/Common.h
	src/ImprovedPerlinNoise.h
	src/VertexDeque.h)
set (${PROJECT_NAME}_sources
	src/main.cpp
	src/Curve.cpp
    src/Character.cpp
	src/Common.cpp
    src/ImprovedPerlinNoise.cpp
    src/VertexDeque.cpp)
set (${PROJECT_NAME}_directories )
set (${PROJECT_NAME}_libraries
	sfml-graphics sfml-window sfml-system)
set (${PROJECT_NAME}_fonts
	res/HelvetiPixel.ttf)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	if (WIN32)
		set(executable_option WIN32)
		find_library(SFML_MAIN_LIB sfml-main.lib)
	endif()
endif()

add_executable(${PROJECT_NAME} ${executable_option}
	${${PROJECT_NAME}_sources}
	${${PROJECT_NAME}_headers})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${${PROJECT_NAME}_directories})

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${${PROJECT_NAME}_libraries})
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	if (WIN32)
		target_link_libraries(${PROJECT_NAME} PRIVATE ${SFML_MAIN_LIB})
	endif()
endif()

# Font file
configure_file(${${PROJECT_NAME}_fonts} . COPYONLY)

message(WARNING "\
If you run directly from executable file `${PROJECT_NAME}[.exe]`, \
you'll need to copy resource file(s) to the same directory with it.
Resource file(s): ${${PROJECT_NAME}_fonts}")