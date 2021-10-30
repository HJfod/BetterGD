#include <tools/keybinds/KeybindManager.hpp>

USE_BGD_NAMESPACE();

bool KeybindManager::init() {
    this->m_sSaveDirectory = "";
    this->m_sSaveFileName = "BGDKeybindManager.json";
    this->m_eSaveType = kBGDSaveTypeJSON;

    this->loadDefaultKeybinds();
    
    return true;
}

KeybindManager::KeybindManager() {
    this->init();
}

KeybindManager::~KeybindManager() {}

KeybindManager* KeybindManager::get() {
    static auto g_manager = new KeybindManager;
    return g_manager;
}

void KeybindManager::saveJSON(nlohmann::json&) {
}

void KeybindManager::loadJSON(nlohmann::json&) {
}

void KeybindManager::loadDefaultKeybinds() {
    // todo (just copy these from BE ig...)
}

bool KeybindManager::addKeybindAction(
    KeybindAction const& ogAction,
    KeybindList   const& defaults,
    keybind_id    const& insertAfter
) {
    auto action = new KeybindAction(ogAction);
    if (!action) return false;
    if (this->m_mActions.count(action->id)) {
        delete action;
        return false;
    }
    action->defaults = defaults;
    m_mActions.insert({ action->id, action });
    if (this->m_mLoadedBinds.count(action->id)) {
        for (auto const& bind : this->m_mLoadedBinds[action->id]) {
            this->addKeybind(action->id, bind);
        }
    } else {
        for (auto const& bind : defaults) {
            this->addKeybind(action->id, bind);
        }
    }
    return true;
}

void KeybindManager::addKeybind(
    keybind_id const& id,
    Keybind const& bind
) {
    if (this->m_mKeybinds.count(bind) && this->m_mKeybinds[bind].size()) {
        this->m_mKeybinds[bind].push_back(id);
    } else {
        this->m_mKeybinds[bind] = { id };
    }
}

void KeybindManager::removeKeybind(
    keybind_id const& id,
    Keybind const& bind
) {
    if (this->m_mKeybinds.count(bind) && this->m_mKeybinds[bind].size()) {
        vector_erase<keybind_id>(this->m_mKeybinds[bind], [id](keybind_id const& kid) -> bool {
            return kid == id;
        });
    }
}

void KeybindManager::replaceKeybind(
    keybind_id const& id,
    Keybind const& bind,
    Keybind const& replBind
) {
    this->removeKeybind(id, bind);
    this->addKeybind(id, replBind);
}

void KeybindManager::clearKeybinds(
    keybind_id const& action
) {
    for (auto & [key, vals] : m_mKeybinds) {
        auto ix = 0;
        for (auto & val : vals) {
            if (val == action) {
                vals.erase(vals.begin() + ix);
                ix--;
                if (!vals.size())
                    m_mKeybinds.erase(key);
            }
            ix++;
        }
    }
}

void KeybindManager::clearActions(
    Keybind const& bind
) {
    if (bind.key != KEY_None && m_mKeybinds.count(bind))
        m_mKeybinds.erase(bind);
}

void KeybindManager::resetToDefault(keybind_id const& actionID, KeybindResetFlags resetType) {
    auto action = this->m_mActions[actionID];
    RepeatableAction* mod = nullptr;
    if (
        (resetType & kfKeybindResetModifierSettings) &&
        (mod = dynamic_cast<RepeatableAction*>(action))
    ) {
        // do the other resets
        mod->repeatChanged = false;
    }
    if (resetType & kfKeybindResetAll) {
        resetType |= kfKeybindResetWithDefaults;
        resetType |= kfKeybindResetWithoutDefaults;
    }
    if (
        (
            (resetType & kfKeybindResetWithDefaults) ||
            (resetType & kfKeybindResetWithoutDefaults)
        ) &&
        this->m_mActions.count(actionID)
    ) {
        if (!(
            (resetType & kfKeybindResetWithoutDefaults) ||
            action->defaults.size()
        )) return;

        this->clearKeybinds(actionID);
        for (auto const& bind : action->defaults) {
            this->addKeybind(actionID, bind);
        }
    }
}

void KeybindManager::resetAllToDefault(KeybindResetFlags resetType) {
    for (auto const& [action, _] : this->m_mActions) {
        this->resetToDefault(action, resetType);
    }
}

decltype(KeybindManager::m_mKeybinds) KeybindManager::getAllKeybinds() {
    return this->m_mKeybinds;
}

KeybindActionList KeybindManager::getAllActions(
    keybind_category_id const& categoryFilter
) {
    if (!categoryFilter) {
        return map_values(this->m_mActions);
    }
    return map_select_all<keybind_id, KeybindAction*>(
        this->m_mActions,
        [categoryFilter](KeybindAction* action) -> bool {
            return action->category == categoryFilter;
        }
    );
}

KeybindActionList KeybindManager::getAllActionsForKeybind(
    keybind_category_id const& category, 
    Keybind const& bind
) {
    if (!this->m_mKeybinds.count(bind)) {
        return {};
    }
    KeybindActionList res;
    for (auto & actionID : m_mKeybinds[bind]) {
        auto action = this->m_mActions[actionID];
        if (action->category == category) {
            res.push_back(action);
        }
    }
    return res;
}

void KeybindManager::handleKeyEvent(
    keybind_category_id const& category,
    Keybind             const& bind,
    CCNode*                    context,
    bool                       down
) {
    if (!this->m_mKeybinds.count(bind)) {
        return;
    }
    for (auto const& id : this->m_mKeybinds[bind]) {
        this->invokeAction(id, category, context, down);
    }
}

void KeybindManager::invokeAction(keybind_id const& id, CCNode* context, bool down) {
    if (this->m_mActions.count(id)) {
        auto action = dynamic_cast<TriggerableAction*>(this->m_mActions[id]);
        if (action) {
            action->invoke(context, down);
        }
    }
}

void KeybindManager::invokeAction(keybind_id const& id, keybind_category_id const& category, CCNode* context, bool down) {
    if (this->m_mActions.count(id)) {
        auto action = dynamic_cast<TriggerableAction*>(this->m_mActions[id]);
        if (action) {
            action->invoke(context, category, down);
        }
    }
}

bool KeybindManager::isModifierPressed(keybind_id const& id) {

}
