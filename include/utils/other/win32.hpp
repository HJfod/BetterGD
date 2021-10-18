#pragma once

#include <BGDMacros.hpp>

#ifdef BGD_IS_WIN32

#include <Windows.h>

struct GLFWwindow;

namespace bgd {
    HWND glfwGetWin32Window(GLFWwindow* window);
    HWND getGDHWND();
}

#endif
