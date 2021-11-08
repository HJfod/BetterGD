#include "NativeUIElementWin32.hpp"
#include "NativeUIBasic.hpp"

#ifdef BGD_IS_WIN32

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

HWND NativeUIElementWin32::hwnd() const {
    return this->m_hwnd;
}

void NativeUIElementWin32::update_pos(int x, int y) {
    SetWindowPos(
        this->m_hwnd,
        reinterpret_cast<HWND>(HWND_TOP),
        x, y,
        0, 0,
        SWP_NOSIZE
    );
}

void NativeUIElementWin32::update_size(int w, int h) {
    SetWindowPos(
        this->m_hwnd,
        reinterpret_cast<HWND>(HWND_TOP),
        0, 0,
        w, h,
        SWP_NOMOVE
    );
}

NativeUIElementWin32::NativeUIElementWin32(NativeUIElementWin32* parent)
    : NativeUIElementBase(parent) {}

void INativeUITextableWin32::update_text(std::string const& text) {
    SetWindowTextA(
        dynamic_cast<NativeUIElementWin32*>(this)->hwnd(),
        text.c_str()
    );
}

void INativeUIColorableWin32::update_color(ccColor3B const& color) {
    SetTextColor(
        reinterpret_cast<HDC>(dynamic_cast<NativeUIElementWin32*>(this)->hwnd()),
        RGB(color.r, color.g, color.b)
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
        reinterpret_cast<HMENU>(this->m_uniqueID),
        (HINSTANCE)GetWindowLongPtr(parent->hwnd(), GWLP_HINSTANCE), 
        nullptr
    );
}

NativeUIText::NativeUIText(NativeUIElement* parent) : NativeUIElement(parent) {
    this->m_hwnd = CreateWindowA( 
        "STATIC",
        this->m_text.c_str(),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        this->m_pos.x,
        this->m_pos.y,
        this->m_size.width,
        this->m_size.height,
        parent->hwnd(),
        reinterpret_cast<HMENU>(this->m_uniqueID),
        (HINSTANCE)GetWindowLongPtr(parent->hwnd(), GWLP_HINSTANCE), 
        nullptr
    );
}

#endif
