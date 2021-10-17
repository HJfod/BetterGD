#pragma once

#include "../../bgd/BGDMacros.hpp"

#ifdef BGD_IS_WIN32

#include <Windows.h>

class GLFWwindow;

namespace bgd {
    HWND glfwGetWin32Window(GLFWwindow* window);
    HWND getGDHWND();
}

#endif
