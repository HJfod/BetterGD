#pragma once

#include "NativeUI.hpp"

#ifdef BGD_IS_WIN32

class NativeUIPlatformWin32 : public NativeUIPlatformBase<NativeUIPlatformWin32> {
    protected:
        HWND m_hwnd;
    
    public:
        HWND hwnd() const {
            return m_hwnd;
        }
};

using NativeUIPlatform = NativeUIPlatformWin32;

class NativeUIWin32 :
    public NativeUIPlatform,
    public NativeUIBase<NativeUIWin32>
{
    public:
        void render() override {

        }

        void show() override {

        }

        void hide() override {

        }

        NativeUIWin32(HWND hwnd) {
            this->m_hwnd = hwnd;
        }
        NativeUIWin32() {
            this->m_hwnd = getGDHWND();
        }
        ~NativeUIWin32() {

        }
};

using NativeUI = NativeUIWin32;

#endif
