#include "NativeUIElement.hpp"

void NativeUIElementBase::attach(NativeUIElementBase* element) {
    if (!element->parent()) {
        this->m_children.push_back(element);
    }
}

void NativeUIElementBase::detach(NativeUIElementBase* element, bool free) {
    vector_erase(this->m_children, element);

    if (free) {
        delete element;
    }
}

std::vector<NativeUIElementBase*> NativeUIElementBase::children() const {
    return this->m_children;
}

NativeUIElementBase* NativeUIElementBase::parent() const {
    return this->m_parent;
}

void NativeUIElementBase::pos(Point pos) {
    this->pos(pos.x, pos.y);
}

void NativeUIElementBase::pos(int x, int y) {
    this->m_pos = { x, y };
    auto [ rx, ry ] = this->real_pos();
    this->update_pos(rx, ry);
}

Point NativeUIElementBase::pos() const {
    return this->m_pos;
}

Point NativeUIElementBase::real_pos() const {
    if (this->m_parent) {
        return this->m_parent->real_pos() + this->m_pos;
    }
    return this->m_pos;
}

void NativeUIElementBase::size(Size size) {
    return this->size(size.width, size.height);
}

void NativeUIElementBase::size(int w, int h) {
    this->m_size = { w, h };
    this->update_size(w, h);
}

Size NativeUIElementBase::size() const {
    return this->m_size;
}

bool NativeUIElementBase::visible() const {
    return this->m_visible;
}

void NativeUIElementBase::show() {
    this->m_visible = true;
    this->update_visibility(true);
}

void NativeUIElementBase::hide() {
    this->m_visible = false;
    this->update_visibility(false);
}

void NativeUIElementBase::render() {
    for (auto const& child : this->m_children) {
        this->render();
    }
}

NativeUIElementBase* NativeUIElementBase::element_by_unique_id(unsigned int id) {
    for (auto const& child : this->m_children) {
        if (child->m_uniqueID == id) {
            return child;
        } else {
            auto res = child->element_by_unique_id(id);
            if (res) {
                return res;
            }
        }
    }
    return nullptr;
}

NativeUIElementBase::NativeUIElementBase(NativeUIElementBase* parent) {
    NativeUIElementBase::s_uniqueID++;
    if (parent) {
        parent->attach(this);
    }
}

NativeUIElementBase::~NativeUIElementBase() {
    NativeUIElementBase::s_uniqueID++;
    for (auto const& child : this->m_children) {
        delete child;
    }
}

void INativeUIColorableBase::color(ccColor3B const& color) {
    this->m_color = color;
    this->update_color(color);
}

ccColor3B INativeUIColorableBase::color() const {
    return this->m_color;
}

void INativeUITextableBase::text(std::string const& text) {
    this->m_text = text;
    this->update_text(text);
}

std::string INativeUITextableBase::text() const {
    return this->m_text;
}
