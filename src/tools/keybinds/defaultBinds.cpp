#include <tools/keybinds/KeybindManager.hpp>

USE_BGD_NAMESPACE();

void KeybindManager::loadDefaultKeybinds() {
    this->addKeybindAction(TriggerableAction {
        "Pause",
        "gd.play.pause",
        { KB_PLAY_CATEGORY, KB_EDITOR_CATEGORY },
        [](CCNode* context, keybind_category_id const& category, bool push) -> bool {
            if (!push) return false;
            switch (category) {
                case hash(KB_PLAY_CATEGORY):
                    as<PlayLayer*>(context)->m_uiLayer->onPause(nullptr);
                    break;
                case hash(KB_EDITOR_CATEGORY):
                    as<EditorUI*>(context)->onPause(nullptr);
                    break;
            }
            return false;
        },
        "Pause ingame / in the editor"_s
    }, {{ KEY_Escape, 0 }});

    this->addKeybindAction(TriggerableAction {
        "Jump P1",
        "gd.play.jump_p1",
        { KB_PLAY_CATEGORY, KB_EDITOR_CATEGORY },
        [](CCNode* context, keybind_category_id const& category, bool push) -> bool {
            GJBaseGameLayer* layer = nullptr;
            switch (category) {
                case hash(KB_PLAY_CATEGORY):
                    layer = as<PlayLayer*>(context);
                    break;
                case hash(KB_EDITOR_CATEGORY):
                    layer = as<EditorUI*>(context)->m_pEditorLayer;
                    break;
            }
            if (layer) {
                if (push) {
                    layer->pushButton(0, true);
                } else {
                    layer->releaseButton(0, true);
                }
            }
            return false;
        },
        "Player 1 Jump"_s
    }, {{ KEY_Space, 0 }});

    this->addKeybindAction(TriggerableAction {
        "Jump P2",
        "gd.play.jump_p2",
        { KB_PLAY_CATEGORY, KB_EDITOR_CATEGORY },
        [](CCNode* context, keybind_category_id const& category, bool push) -> bool {
            GJBaseGameLayer* layer = nullptr;
            switch (category) {
                case hash(KB_PLAY_CATEGORY):
                    layer = as<PlayLayer*>(context);
                    break;
                case hash(KB_EDITOR_CATEGORY):
                    layer = as<EditorUI*>(context)->m_pEditorLayer;
                    break;
            }
            if (layer) {
                if (push) {
                    layer->pushButton(0, false);
                } else {
                    layer->releaseButton(0, false);
                }
            }
            return false;
        },
        "Player 2 Jump"_s
    }, {{ KEY_Up, 0 }});

    this->addKeybindAction(TriggerableAction {
        "Place Checkpoint",
        "gd.play.place_checkpoint",
        KB_PLAY_CATEGORY,
        [](CCNode* context, bool push) -> bool {
            if (push) {
                auto pl = as<PlayLayer*>(context);
                if (pl->m_isPracticeMode) {
                    pl->m_uiLayer->onCheck(nullptr);
                }
            }
            return false;
        },
        "Place a Checkpoint in Practice Mode"_s
    }, {{ KEY_Z, 0 }});

    this->addKeybindAction(TriggerableAction {
        "Delete Checkpoint",
        "gd.play.delete_checkpoint",
        KB_PLAY_CATEGORY,
        [](CCNode* context, bool push) -> bool {
            if (push) {
                auto pl = as<PlayLayer*>(context);
                if (pl->m_isPracticeMode) {
                    pl->m_uiLayer->onDeleteCheck(nullptr);
                }
            }
            return false;
        },
        "Delete the last Checkpoint in Practice Mode"_s
    }, {{ KEY_X, 0 }});
}
