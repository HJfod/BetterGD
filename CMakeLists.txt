cmake_minimum_required(VERSION 3.3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME BetterGD)
set(CONFIG_DIR RelWithDebInfo)
set(BUILD_SUBS True)

set(COCOS2D_LOCATION        ${CMAKE_SOURCE_DIR}/submodules/Cocos2d)
set(GD_H_LOCATION           ${CMAKE_SOURCE_DIR}/submodules/gd.h)
set(MINHOOK_LOCATION        ${CMAKE_SOURCE_DIR}/submodules/MinHook)
set(ZIPPER_LOCATION         ${CMAKE_SOURCE_DIR}/submodules/Zipper)
set(LILAC_LOCATION          ${CMAKE_SOURCE_DIR}/submodules/lilac)
set(MINHOOK_BUILD_LOCATION  ${CMAKE_BINARY_DIR}/submodules/MinHook)
set(ZIPPER_BUILD_LOCATION   ${CMAKE_BINARY_DIR}/submodules/Zipper)
set(LILAC_BUILD_LOCATION    ${CMAKE_BINARY_DIR}/submodules/lilac)

set(BGD_PRODUCT_NUMBER 1)
set(BGD_PRODUCT_VERSION 0)
set(BGD_BUILD_NUMBER 0)

project(${PROJECT_NAME} VERSION 1.0.0)

find_package(ZLIB REQUIRED)

message(info ${ZLIB_DEPS})

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

configure_file(
  ${CMAKE_SOURCE_DIR}/src/config.h.in
  ${CMAKE_SOURCE_DIR}/src/config.h
)

set(WIN32 ON)
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

if (${BUILD_SUBS})
  # add_subdirectory(${MINHOOK_LOCATION})
  # target_link_libraries(${PROJECT_NAME} ${MINHOOK_BUILD_LOCATION}/${CONFIG_DIR}/minhook.x32.lib)

  add_subdirectory(${LILAC_LOCATION})
  target_link_libraries(${PROJECT_NAME} ${LILAC_BUILD_LOCATION}/src/${CONFIG_DIR}/lilac_hook.lib)

  set(BUILD_TEST Off CACHE BOOL "Build the test program." FORCE)
  add_subdirectory(${ZIPPER_LOCATION})
  target_link_libraries(${PROJECT_NAME} ${ZIPPER_BUILD_LOCATION}/${CONFIG_DIR}/staticZipper.lib)
endif()

add_compile_definitions(_EXPORTING)

add_subdirectory(${COCOS2D_LOCATION})

include_directories(
  cocos2d
  "${GD_H_LOCATION}/include"
  "${GD_H_LOCATION}"
  "${MINHOOK_LOCATION}/include"
  "${ZIPPER_LOCATION}/zipper"
  "${CMAKE_SOURCE_DIR}/src/bgd"
  "${CMAKE_SOURCE_DIR}/include"
  "${CMAKE_SOURCE_DIR}/include/bgd"
  "${CMAKE_SOURCE_DIR}/include/tools/nodes"
  "${CMAKE_SOURCE_DIR}/include/tools/managers"
  "${CMAKE_SOURCE_DIR}/submodules/lilac/include"
  "${CMAKE_SOURCE_DIR}/submodules/Nuklear"
  ${ZLIB_INCLUDE_DIRS}
)

target_link_libraries(
  ${PROJECT_NAME}
  cocos2d
  ${ZLIB_LIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

add_subdirectory(plugins)

add_subdirectory(installer/move)
