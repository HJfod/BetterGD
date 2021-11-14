#pragma once
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <functional>

namespace ImGuiHook {
    void setupHooks(std::function<void(void*, void*, void**)> hookFunc);
    void setRender(bool yes);

    std::function<void()> getRender();

    void setRenderFunction(std::function<void()> func);
    void setInitFunction(std::function<void()> func);
    void setToggleCallback(std::function<void()> func);
}
