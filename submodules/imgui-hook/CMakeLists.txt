cmake_minimum_required(VERSION 3.0)

add_library(imgui-hook STATIC
    imgui-hook.cpp
    imgui/imgui.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui_tables.cpp
    imgui/imgui_draw.cpp
    imgui/backends/imgui_impl_win32.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)
set_property(TARGET imgui-hook PROPERTY CXX_STANDARD 17)
target_include_directories(imgui-hook PRIVATE
    "imgui"
    "../Cocos2d/cocos2dx/"
    "../Cocos2d/cocos2dx/include"
    "../Cocos2d/cocos2dx/kazmath/include"
    "../Cocos2d/cocos2dx/platform/win32"
    "../Cocos2d/cocos2dx/platform/third_party/win32"
    "../Cocos2d/cocos2dx/platform/third_party/win32/OGLES"
)
target_link_libraries(
    imgui-hook PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../Cocos2d/cocos2dx/libcocos2d.lib"
    opengl32
)
