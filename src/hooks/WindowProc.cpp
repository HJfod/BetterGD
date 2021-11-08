#include "bgd_hook.hpp"
#include "../src/devtools/DevTools.hpp"

LRESULT CCEGLView_WindowProc(void* ecx, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND: {
            if (DevTools::get()->ui()) {
                auto element = dynamic_cast<NativeUIButton*>(
                    DevTools::get()->ui()->element_by_unique_id(LOWORD(wParam))
                );
                if (element) {
                    element->invoke();
                    return DefWindowProc(hwnd, msg, wParam, lParam);
                }
            }
        } break;

        case WM_CTLCOLORSTATIC: {
            std::cout << "WM_CTLCOLORSTATIC\n"; 
            if (DevTools::get()->ui()) {
                std::cout << "ui exist\n"; 
                auto element = dynamic_cast<INativeUIColorable*>(
                    DevTools::get()->ui()->element_by_unique_id(LOWORD(wParam))
                );
                if (element) {
                    std::cout << "found colorable\n"; 
                    element->update_color(element->color());
                    return (BOOL)GetSysColorBrush(COLOR_MENU);
                }
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
