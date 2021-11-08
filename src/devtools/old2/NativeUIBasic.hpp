#pragma once

#include "NativeUIElement.hpp"

class NativeUIButton;

using NativeUICallback = std::function<void(NativeUIButton*)>;

class NativeUIButton : public NativeUIElement, public INativeUITextable {
    protected:
        NativeUICallback m_callback = nullptr;

    public:
        NativeUIButton(NativeUIElement* parent);

        void callback(NativeUICallback callback);

        void invoke();
};

class NativeUIText : public NativeUIElement, public INativeUITextable, public INativeUIColorable {
    public:
        NativeUIText(NativeUIElement* parent);
};
