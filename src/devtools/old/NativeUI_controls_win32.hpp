#pragma once

#include "NativeUI.hpp"
#include <optional>

#ifdef BGD_IS_WIN32

class NativeUIButtonWin32 : public NativeUIButtonBase<NativeUIButtonWin32> {
    public:
        void update_pos(float x, float y) override {
            SetWindowPos(
                this->m_hwnd,
                reinterpret_cast<HWND>(HWND_TOP),
                static_cast<int>(x),
                static_cast<int>(y),
                0, 0,
                SWP_NOSIZE
            );
        }

        void update_size(float w, float h) override {
            SetWindowPos(
                this->m_hwnd,
                reinterpret_cast<HWND>(HWND_TOP),
                0, 0,
                static_cast<int>(w),
                static_cast<int>(h),
                SWP_NOMOVE
            );
        }

        void update_text(std::string const& text) override {
            SetWindowTextA(
                this->m_hwnd,
                text.c_str()
            );
        }

        NativeUIButtonWin32() {}

        template<class Element>
        NativeUIButtonWin32(
            INativeUIContainer<Element>& parent,
            std::optional<CCPoint> const& pos  = CCPointZero,
            std::optional<CCSize>  const& size = CCSizeZero,
            std::string            const& text = ""
        ) {
            if (pos.has_value()) {
                this->m_obPosition = pos.value();
            }
            if (size.has_value()) {
                this->m_obSize = size.value();
            }
            auto hwnd = reinterpret_cast<NativeUIPlatform&>(parent).hwnd();
            this->m_hwnd = CreateWindowA( 
                "BUTTON",
                text.c_str(),
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                static_cast<int>(this->m_obPosition.x),
                static_cast<int>(this->m_obPosition.y),
                static_cast<int>(this->m_obSize.width),
                static_cast<int>(this->m_obSize.height),
                hwnd,
                nullptr,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
                nullptr
            );
        }
};

using NativeUIButton = NativeUIButtonWin32;

#endif
