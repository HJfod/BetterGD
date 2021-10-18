#include <utils/gd/include_gd.hpp>
#include <utils/other/platform.hpp>

#ifdef BGD_IS_WIN32

USE_BGD_NAMESPACE();

HWND bgd::glfwGetWin32Window(GLFWwindow* window) {
    static auto cocosBase = GetModuleHandleA("libcocos2d.dll");

    auto pRet = reinterpret_cast<HWND(__cdecl*)(GLFWwindow*)>(
        reinterpret_cast<uintptr_t>(cocosBase) + 0x112c10
    )(window);

    return pRet;
}

HWND bgd::getGDHWND() {
    static HWND g_hwnd = nullptr;

    if (!g_hwnd) {
        auto dir = CCDirector::sharedDirector();
        if (!dir) return nullptr;
        auto opengl = dir->getOpenGLView();
        if (!opengl) return nullptr;
        auto wnd = dir->getOpenGLView()->getWindow();
        if (!wnd) return nullptr;
        g_hwnd = glfwGetWin32Window(wnd);
    }

    return g_hwnd;
}

#endif
