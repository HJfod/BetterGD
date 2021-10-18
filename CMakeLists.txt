cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME BetterGD)

set(GD_H_LOCATION C:/Users/HJfod/Documents/github/GDMake/bin/Debug/net5.0/win10-x64/submodules)
set(COCOS2D_LOCATION C:/Users/HJfod/Documents/github/GDMake/bin/Debug/net5.0/win10-x64/submodules)

set(BGD_PRODUCT_NUMBER 1)
set(BGD_PRODUCT_VERSION 0)
set(BGD_BUILD_NUMBER 0)

project(${PROJECT_NAME} VERSION 1.0.0)

file(GLOB_RECURSE SOURCES 
    src/*.cpp
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
)

set(WIN32 ON)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${COCOS2D_LOCATION}/cocos2dx
    ${COCOS2D_LOCATION}/cocos2dx/include
    ${COCOS2D_LOCATION}/cocos2dx/kazmath/include
    ${COCOS2D_LOCATION}/cocos2dx/platform/third_party/win32/OGLES
    ${COCOS2D_LOCATION}/cocos2dx/platform/win32
    ${COCOS2D_LOCATION}/extensions
    ${GD_H_LOCATION}/gd/include
    ${GD_H_LOCATION}/gd
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/bgd
)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_subdirectory(example)

