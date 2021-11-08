#include "bgd_hook.hpp"
#include "../src/devtools/DevTools.hpp"

LRESULT CCEGLView_WindowProc(void* ecx, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND: {
            auto element = dynamic_cast<NativeUIButton*>(
                DevTools::get()->ui()->element_by_unique_id(LOWORD(wParam))
            );
            if (element) {
                element->invoke();
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        } break;

        case WM_CTLCOLORSTATIC: {
            auto element = dynamic_cast<INativeUIColorable*>(
                DevTools::get()->ui()->element_by_unique_id(LOWORD(wParam))
            );
            if (element) {
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }
        } break;

        case WM_COPYDATA: {
            
        } break;
    }
    return hook::orig<&CCEGLView_WindowProc>(ecx, hwnd, msg, wParam, lParam);
}
static InternalCreateHook<&CCEGLView_WindowProc>$ccevwp(
    "libcocos2d.dll", 0x113090
);
