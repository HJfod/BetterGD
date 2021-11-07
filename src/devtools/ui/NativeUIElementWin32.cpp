#include "NativeUIElementWin32.hpp"
#include "NativeUIBasic.hpp"

#ifdef BGD_IS_WIN32

HWND NativeUIElementWin32::hwnd() const {
    return this->m_hwnd;
}

NativeUIElementWin32::NativeUIElementWin32(NativeUIElementWin32* parent)
    : NativeUIElementBase(parent) {}

void NativeUIButton::update_pos(int x, int y) {
    SetWindowPos(
        this->m_hwnd,
        reinterpret_cast<HWND>(HWND_TOP),
        x, y,
        0, 0,
        SWP_NOSIZE
    );
}

void NativeUIButton::update_size(int w, int h) {
    SetWindowPos(
        this->m_hwnd,
        reinterpret_cast<HWND>(HWND_TOP),
        0, 0,
        w, h,
        SWP_NOMOVE
    );
}

void NativeUIButton::update_text(std::string const& text) {
    SetWindowTextA(
        this->m_hwnd,
        text.c_str()
    );
}

void NativeUIButton::invoke() {
    if (this->m_callback) {
        this->m_callback(this);
    }
    SetFocus(nullptr);
}

void NativeUIButton::callback(NativeUICallback callback) {
    this->m_callback = callback;
}

NativeUIButton::NativeUIButton(NativeUIElement* parent) : NativeUIElement(parent) {
    this->m_hwnd = CreateWindowA( 
        "BUTTON",
        this->m_text.c_str(),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        this->m_pos.x,
        this->m_pos.y,
        this->m_size.width,
        this->m_size.height,
        parent->hwnd(),
        nullptr,
        (HINSTANCE)GetWindowLongPtr(parent->hwnd(), GWLP_HINSTANCE), 
        nullptr
    );
}

#endif
