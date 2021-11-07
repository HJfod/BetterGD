#pragma once

#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

using NativeUIHandle = void*;

template<class Element>
class INativeUIContainer;

template<class Platform>
class NativeUIPlatformBase {};

class NativeUIElement {
    protected:
        INativeUIContainer<NativeUIElement>* m_pParent = nullptr;

        template<class CElement>
        friend class INativeUIContainer;

    public:
        bool has_parent() const {
            return this->m_pParent;
        }
        INativeUIContainer<NativeUIElement>* parent() const {
            return this->m_pParent;
        }

        virtual ~NativeUIElement() = default;
};

template<class Element>
class INativeUIContainer {
    protected:
        std::vector<NativeUIElement*> m_vChildren;
    
    public:
        Element& add(NativeUIElement& element) {
            this->m_vChildren.push_back(&element);
            element.m_pParent = reinterpret_cast<INativeUIContainer<NativeUIElement>*>(this);
            return static_cast<Element&>(*this);
        }
        Element& remove(NativeUIElement const& element) {
            vector_erase(this->m_vChildren, &element);
            element.m_pParent = nullptr;
            return static_cast<Element&>(*this);
        }
        std::vector<NativeUIElement*> children() {
            return this->m_vChildren;
        }

        virtual ~INativeUIContainer() = default;
};

template<class Element>
class INativeUIPositionable {
    protected:
        CCPoint m_obPosition = CCPointZero;

        virtual void update_pos(float x, float y) = 0;
    
    public:
        Element& pos(CCPoint const& pos) {
            return this->pos(pos.x, pos.y);
        }
        Element& pos(float x, float y) {
            this->m_obPosition.x = x;
            this->m_obPosition.y = y;
            auto [rx, ry] = this->real_pos();
            this->update_pos(rx, ry);
            return static_cast<Element&>(*this);
        }
        CCPoint const& pos() const {
            return this->m_obPosition;
        }
        CCPoint const& real_pos() const {
            const auto elem = dynamic_cast<const NativeUIElement&>(*this);
            if (elem.has_parent() && dynamic_cast<INativeUIPositionable*>(elem.parent())) {
                return reinterpret_cast<INativeUIPositionable*>(elem.parent())->real_pos() + this->pos();
            }
            return this->pos();
        }

        virtual ~INativeUIPositionable() = default;
};

template<class Element>
class INativeUISizeable {
    protected:
        CCSize m_obSize = CCSizeZero;

        virtual void update_size(float w, float h) = 0;
    
    public:
        Element& size(CCSize const& size) {
            return this->size(size.width, size.height);
        }
        Element& size(float w, float h) {
            this->m_obSize.width = w;
            this->m_obSize.height = h;
            this->update_size(w, h);
            return static_cast<Element&>(*this);
        }
        CCSize const& size() const {
            return this->m_obSize;
        }

        virtual ~INativeUISizeable() = default;
};

template<class Element>
class INativeUITextable {
    protected:
        std::string m_sText;

        virtual void update_text(std::string const& text) = 0;
    
    public:
        Element& text(std::string const& text) {
            this->m_sText = text;
            this->update_text(text);
            return static_cast<Element&>(*this);
        }
        std::string const& text() const {
            return this->m_sText;
        }

        virtual ~INativeUITextable() = default;
};

template<class Platform>
class NativeUIBase : public INativeUIContainer<Platform> {
    public:
        virtual void render() {}
        virtual void show() {}
        virtual void hide() {}

        virtual ~NativeUIBase() = default;
};

#include "NativeUI_win32.hpp"

template<class Platform>
class NativeUIButtonBase :
    public NativeUIPlatform,
    public NativeUIElement,
    public INativeUIPositionable<Platform>,
    public INativeUISizeable<Platform>,
    public INativeUITextable<Platform>
{};

#include "NativeUI_controls_win32.hpp"
