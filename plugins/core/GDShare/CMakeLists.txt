cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME GDShare)

project(${PROJECT_NAME} VERSION 1.0.0)

file(GLOB_RECURSE SOURCES 
  *.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set(WIN32 ON)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".bgd")

target_link_libraries(
    ${PROJECT_NAME}
    BetterGD
)

