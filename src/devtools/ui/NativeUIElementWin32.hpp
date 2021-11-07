#pragma once

#include "NativeUIElement.hpp"

#ifdef BGD_IS_WIN32

class NativeUIElementWin32 : public NativeUIElementBase {
    protected:
        HWND m_hwnd = nullptr;
    
    public:
        HWND hwnd() const;

        NativeUIElementWin32(NativeUIElementWin32*);
};

using NativeUIElement = NativeUIElementWin32;

#endif
