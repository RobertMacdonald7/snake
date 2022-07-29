cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_NAME "snake")

project(${PROJECT_NAME})

set(EXECUTABLE_NAME ${PROJECT_NAME})

set(SNAKE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/GameClient")
set(SNAKE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/GameClient")

file(GLOB_RECURSE SOURCES "${SNAKE_SOURCE_DIR}/*.cpp")

add_executable(${EXECUTABLE_NAME} ${SOURCES})

if(CMAKE_BUILD_TYPE MATCHES Debug)
	target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
endif()

if(MSVC)
	# Enable multi-threaded compilation.
	target_compile_options(${PROJECT_NAME} PRIVATE "/MP")

	if(CMAKE_BUILD_TYPE MATCHES Debug)
		target_compile_options(${PROJECT_NAME} PRIVATE "/MDd")
	else()
		target_compile_options(${PROJECT_NAME} PRIVATE "/MD")
	endif()

	target_compile_options(${PROJECT_NAME} PRIVATE "/D_UNICODE")
	target_compile_options(${PROJECT_NAME} PRIVATE "/DUNICODE")

	target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")

	# Enable severe warnings.
	target_compile_options(${PROJECT_NAME} PRIVATE "/W1")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "${SNAKE_INCLUDE_DIR}")
