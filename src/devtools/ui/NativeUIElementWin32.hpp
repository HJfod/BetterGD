#pragma once

#include "NativeUIElement.hpp"

#ifdef BGD_IS_WIN32

class NativeUIElementWin32 : public NativeUIElementBase {
    protected:
        HWND m_hwnd = nullptr;
    
        void update_pos (int x, int y) override;
        void update_size(int w, int h) override;

    public:
        HWND hwnd() const;

        NativeUIElementWin32(NativeUIElementWin32*);
};

using NativeUIElement = NativeUIElementWin32;

class INativeUITextableWin32 : public INativeUITextableBase {
    protected:
        void update_text(std::string const& text) override;
};

using INativeUITextable = INativeUITextableWin32;

class INativeUIColorableWin32 : public INativeUIColorableBase {
    protected:
        void update_color(ccColor3B const& color) override;
};

using INativeUIColorable = INativeUIColorableBase;

#endif
