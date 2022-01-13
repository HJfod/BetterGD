cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME BetterGD)

project(${PROJECT_NAME} VERSION 1.0.0)

file(GLOB_RECURSE SOURCES 
	src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

add_compile_definitions(_EXPORTING_BGD)

set(WIN32 ON)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_subdirectory(submodules/lilac)

create_lilac_file(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/api"
    "${CMAKE_CURRENT_SOURCE_DIR}/api/nodes"
    "${CMAKE_CURRENT_SOURCE_DIR}/api/managers"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(
    ${PROJECT_NAME}
    lilac
)

add_subdirectory(mods)
