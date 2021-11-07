#include "bgd_hook.hpp"
#include "../src/devtools/DevTools.hpp"

LRESULT __stdcall CCEGLView_WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_COMMAND) {
        auto element = dynamic_cast<NativeUIButton*>(
            DevTools::get()->ui()->element_by_unique_id(LOWORD(wParam))
        );
        if (element) {
            element->invoke();
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

    return hook::orig<&CCEGLView_WindowProc>(hwnd, msg, wParam, lParam);
}

static InternalCreateHook<&CCEGLView_WindowProc, hook::Thiscall>$ccevwp(
    "libcocos2d.dll", 0x113090
);
