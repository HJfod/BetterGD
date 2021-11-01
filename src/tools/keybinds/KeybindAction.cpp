#include <tools/keybinds/KeybindAction.hpp>

USE_BGD_NAMESPACE();

bool KeybindAction::operator==(KeybindAction const& other) const {
    return this->id == other.id;
}
KeybindAction::~KeybindAction() {}
KeybindAction* KeybindAction::copy() const {
    return new KeybindAction(*this);
}
bool KeybindAction::inCategory(keybind_category_id const& category) const {
    return this->categories.count(category);
}

KeybindModifier::~KeybindModifier() {}

KeybindModifier::KeybindModifier(
    std::string         const& name,
    keybind_action_id   const& id,
    keybind_category_id const& category,
    std::string         const& subcategory,
    std::string         const& description
) {
    this->name = name;
    this->id = id;
    this->categories = { category };
    this->subcategory = subcategory;
    this->description = description;
}

KeybindModifier::KeybindModifier(
    std::string         const& name,
    keybind_action_id   const& id,
    decltype(KeybindAction::categories) const& categories,
    std::string         const& subcategory,
    std::string         const& description
) {
    this->name = name;
    this->id = id;
    this->categories = categories;
    this->subcategory = subcategory;
    this->description = description;
}

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
KeybindAction* TriggerableAction::copy() const {
    return new TriggerableAction(*this);
}

TriggerableAction::TriggerableAction(
    std::string         const& name,
    keybind_action_id   const& id,
    keybind_category_id const& category,
    decltype(TriggerableAction::action) action
) {
    this->name = name;
    this->id = id;
    this->categories = { category };
    this->action = action;
}

TriggerableAction::TriggerableAction(
    std::string         const& name,
    keybind_action_id   const& id,
    keybind_category_id const& category,
    decltype(TriggerableAction::action) action,
    std::string         const& description
) {
    this->name = name;
    this->id = id;
    this->categories = { category };
    this->description = description;
    this->action = action;
}

TriggerableAction::TriggerableAction(
    std::string         const& name,
    keybind_action_id   const& id,
    keybind_category_id const& category,
    decltype(TriggerableAction::actionWithID) action,
    std::string         const& description
) {
    this->name = name;
    this->id = id;
    this->categories = { category };
    this->description = description;
    this->actionWithID = action;
}

TriggerableAction::TriggerableAction(
    std::string         const& name,
    keybind_action_id   const& id,
    std::unordered_set<keybind_category_id> const& categories,
    decltype(actionWithID)     action,
    std::string         const& description
) {
    this->name = name;
    this->id = id;
    this->categories = categories;
    this->description = description;
    this->actionWithID = action;
}

TriggerableAction::TriggerableAction(
    std::string         const& name,
    keybind_action_id   const& id,
    keybind_category_id const& category,
    std::string         const& subcategory,
    decltype(TriggerableAction::action) action,
    std::string         const& description
) {
    this->name = name;
    this->id = id;
    this->categories = { category };
    this->subcategory = subcategory;
    this->description = description;
    this->action = action;
}

TriggerableAction::TriggerableAction(
    std::string         const& name,
    keybind_action_id   const& id,
    keybind_category_id const& category,
    std::string         const& subcategory,
    decltype(TriggerableAction::actionWithID) action,
    std::string         const& description
) {
    this->name = name;
    this->id = id;
    this->categories = { category };
    this->subcategory = subcategory;
    this->description = description;
    this->actionWithID = action;
}

RepeatableAction::~RepeatableAction() {}
KeybindAction* RepeatableAction::copy() const {
    return new RepeatableAction(*this);
}

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
keybind_category_id::operator int() const {
    return hash(this->value);
}
keybind_category_id::operator std::string() const {
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

std::ostream& bgd::operator<<(std::ostream& stream, keybind_category_id const& id) {
    return stream << id.value;
}

std::size_t std::hash<keybind_category_id>::operator()(keybind_category_id const& category) const {
    return std::hash<decltype(category.value)>()(category.value);
}
