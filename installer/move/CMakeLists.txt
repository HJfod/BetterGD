cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(move move.cpp)

if(DEFINED ENV{GD_LOCATION})
message(STATUS "Environment variable GD_LOCATION found, moving files automatically after build")
add_custom_command(
  TARGET move POST_BUILD
  COMMAND move $ENV{GD_LOCATION} "${CMAKE_SOURCE_DIR}/build" ${CONFIG_DIR}
)
else()
message(STATUS "Environment variable GD_LOCATION not found, files will have to be moved manually")
endif()

