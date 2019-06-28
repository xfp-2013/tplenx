cmake_minimum_required(VERSION 3.4)

# Workspace name
project(tplenx)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_INCLUDE_DIR
	${CMAKE_SOURCE_DIR}/include
)

link_directories(
	${CMAKE_SOURCE_DIR}/lib
)


if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

execute_process(
	COMMAND git rev-parse --abbrev-ref HEAD
	WORKING_DIRECTORY	${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE		GIT_BRANCH
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DGIT_BRANCH=\"${GIT_BRANCH}\"")

execute_process(
	COMMAND git log -1 --format=%h
	WORKING_DIRECTORY	${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE		GIT_COMMIT
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DGIT_COMMIT=\"${GIT_COMMIT}\"")

execute_process(
	COMMAND git rev-list HEAD --count
	WORKING_DIRECTORY	${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE		GIT_COUNT
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DGIT_COUNT=${GIT_COUNT}")

execute_process(
	COMMAND date "+%s"
	WORKING_DIRECTORY	${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE		BUILD_TIME
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions("-DBUILD_TIME=${BUILD_TIME}")

# Compiler options
add_definitions(-Wall)
add_definitions(-Wno-reorder)
add_definitions(-Wno-unused-function)
add_definitions(-Wno-unused-variable)
add_definitions(-Wno-unused-but-set-variable)
add_definitions(-Wno-unused-result)
add_definitions(-Wno-format-security)
add_definitions(-Wno-unused-local-typedefs)
add_definitions(
	-D_FILE_OFFSET_BITS=64
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__SFILE__='\"$(notdir $(basename $<))\"'")

# Set default locations
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Projects
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
