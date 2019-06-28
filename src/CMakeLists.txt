cmake_minimum_required(VERSION 3.4)

project(tplenx VERSION 1.0)

include_directories(
	${CMAKE_INCLUDE_DIR}
)
link_directories(
	${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)

add_definitions("-DPROJECT_NAME=\"${PROJECT_NAME}\"")

message(STATUS "Version: ${PROJECT_NAME} ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${GIT_COUNT}")


file(GLOB CXX_SRCS *.cpp)
add_executable(tplenx ${CXX_SRCS})
set_target_properties(tplenx PROPERTIES LINKER_LANGUAGE CXX)
set_property(TARGET tplenx PROPERTY CXX_STANDARD 17)
set_property(TARGET tplenx PROPERTY CXX_STANDARD_REQUIRED TRUE)
set_property(TARGET tplenx PROPERTY CMAKE_EXE_LINKER_FLAGS “${CMAKE_EXE_LINKER_FLAGS} -static”)

target_link_libraries(tplenx
)
