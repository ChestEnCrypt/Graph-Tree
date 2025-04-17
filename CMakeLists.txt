cmake_minimum_required(VERSION 3.16)

project(Graph_Tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
	src/main.cpp
)

add_executable(Graph_Tree ${SOURCES})

set_target_properties(Graph_Tree PROPERTIES FOLDER "EXECUTABLE")

set(SFML_DIR "${CMAKE_SOURCE_DIR}/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

target_link_libraries(Graph_Tree PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

target_include_directories(Graph_Tree PRIVATE "${CMAKE_SOURCE_DIR}/include/SFML")
target_include_directories(Graph_Tree PRIVATE "${CMAKE_SOURCE_DIR}/include")

add_custom_command(TARGET Graph_Tree POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/bin $<TARGET_FILE_DIR:Graph_Tree>)
