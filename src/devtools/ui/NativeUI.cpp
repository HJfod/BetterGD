#include "NativeUI.hpp"

NativeUI::NativeUI() : NativeUIElement(nullptr) {
    this->m_hwnd = getGDHWND();
}
