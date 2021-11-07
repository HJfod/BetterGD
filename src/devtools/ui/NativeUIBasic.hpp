#pragma once

#include "NativeUIElement.hpp"

class NativeUIButton;

using NativeUICallback = std::function<void(NativeUIButton*)>;

class NativeUIButton : public NativeUIElement, public INativeUITextable {
    protected:
        NativeUICallback m_callback = nullptr;

        void update_pos (int x, int y) override;
        void update_size(int w, int h) override;
        void update_text(std::string const& text) override;

    public:
        NativeUIButton(NativeUIElement* parent);

        void callback(NativeUICallback callback);

        void invoke();
};
