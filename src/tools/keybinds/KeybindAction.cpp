#include <tools/keybinds/KeybindAction.hpp>

USE_BGD_NAMESPACE();

bool KeybindAction::operator==(KeybindAction const& other) const {
    return this->id == other.id;
}
KeybindAction::~KeybindAction() {}

KeybindModifier::~KeybindModifier() {}

void TriggerableAction::invoke(CCNode* node, bool down) const {
    if (this->action) {
        this->action(node, down);
    }
}
void TriggerableAction::invoke(CCNode* node, keybind_category_id const& id, bool down) const {
    if (this->actionWithID) {
        this->actionWithID(node, id, down);
    } else if (this->action) {
        this->action(node, down);
    }
}
TriggerableAction::~TriggerableAction() {}

RepeatableAction::~RepeatableAction() {}

keybind_category_id::keybind_category_id() {
    value = nullptr;
}
keybind_category_id::keybind_category_id(const char* val) {
    value = val;
}
keybind_category_id::keybind_category_id(std::string const& val) {
    *this = val;
}
keybind_category_id::~keybind_category_id() {
    delete[] value;
}
const char* keybind_category_id::c_str() const {
    if (value) {
        return value;
    } else {
        return "";
    }
}
keybind_category_id::operator bool() const {
    return this->value;
}
keybind_category_id keybind_category_id::operator=(std::string const& val) {
    if (value) {
        delete[] value;
    }
    value = _strdup(val.c_str());
    return *this;
}
bool keybind_category_id::operator==(keybind_category_id const& other) const {
    return
        string_to_lower(this->value) ==
        string_to_lower(other.value);
}
