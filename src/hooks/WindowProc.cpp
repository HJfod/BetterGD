#include "bgd_hook.hpp"
#include "../src/devtools/DevTools.hpp"

LRESULT CCEGLView_WindowProc(void* ecx, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COPYDATA: {
            
        } break;
    }
    return hook::orig<&CCEGLView_WindowProc>(ecx, hwnd, msg, wParam, lParam);
}
static InternalCreateHook<&CCEGLView_WindowProc>$ccevwp(
    "libcocos2d.dll", 0x113090
);
