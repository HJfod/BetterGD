cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME CoolScrollbars)

project(${PROJECT_NAME} VERSION 0.1.0)

file(GLOB_RECURSE SOURCES 
  *.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" OUTPUT_NAME "CoolScrollbars")

create_lilac_file(${PROJECT_NAME})

target_link_libraries(
  ${PROJECT_NAME}
  BetterGD
)

