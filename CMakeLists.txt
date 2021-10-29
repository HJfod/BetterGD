cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME BetterGD)

set(COCOS2D_LOCATION submodules/Cocos2d)
set(GD_H_LOCATION submodules/gd.h)
set(MINHOOK_LOCATION submodules/MinHook)
set(ZIPPER_LOCATION submodules/Zipper)

set(BGD_PRODUCT_NUMBER 1)
set(BGD_PRODUCT_VERSION 0)
set(BGD_BUILD_NUMBER 0)

project(${PROJECT_NAME} VERSION 1.0.0)

find_package(ZLIB REQUIRED)

file(GLOB_RECURSE SOURCES 
    src/*.cpp
)

if(CMake_MSVC_PARALLEL)
  if(CMake_MSVC_PARALLEL GREATER 0)
    add_definitions(/MP${CMake_MSVC_PARALLEL})
  else()
    add_definitions(/MP)
  endif()
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
)

set(WIN32 ON)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

add_compile_definitions(_EXPORTING)

include_directories(
    "${COCOS2D_LOCATION}/cocos2dx"
    "${COCOS2D_LOCATION}/cocos2dx/include"
    "${COCOS2D_LOCATION}/cocos2dx/kazmath/include"
    "${COCOS2D_LOCATION}/cocos2dx/platform/third_party/win32/OGLES"
    "${COCOS2D_LOCATION}/cocos2dx/platform/win32"
    "${COCOS2D_LOCATION}/extensions"
    "${GD_H_LOCATION}/include"
    "${GD_H_LOCATION}"
    "${MINHOOK_LOCATION}/include"
    "${ZIPPER_LOCATION}/zipper"
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/bgd"
    "${CMAKE_SOURCE_DIR}/include/tools/nodes"
    "${CMAKE_SOURCE_DIR}/include/tools/managers"
    ${ZLIB_DEPS}
)

target_link_libraries(
    ${PROJECT_NAME}
    "${COCOS2D_LOCATION}/cocos2dx/libcocos2d.lib"
    "${COCOS2D_LOCATION}/extensions/libExtensions.lib"
    "${MINHOOK_LOCATION}/build/Release/minhook.x32.lib"
    "${ZIPPER_LOCATION}/build/Release/staticZipper.lib"
    ZLIB
)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_subdirectory(example)

