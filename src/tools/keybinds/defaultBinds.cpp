#include <tools/keybinds/KeybindManager.hpp>

USE_BGD_NAMESPACE();

#define CLICKED(...) if (down) { __VA_ARGS__ }
#define UI(...) auto ui = as<EditorUI*>(context); __VA_ARGS__
#define NO_PLAY(...) if (ui->m_pEditorLayer->m_ePlaybackMode !=  kPlaybackModePlaying) { __VA_ARGS__ }
#define ACTION(...) __VA_ARGS__
#define EDIT_ACTION(...) CLICKED( UI( NO_PLAY( __VA_ARGS__ ) ) )
#define BINDS(...) { __VA_ARGS__ }
#define BIND(...) { { __VA_ARGS__ } }
#define KB(_mod_, _key_) { KEY_##_key_, Keybind::km##_mod_ }
#define KBS(_mod_, _key_) { { KEY_##_key_, Keybind::km##_mod_ } }

#define ADD_EDITOR_KB(_name_, _id_, _sub_, _func_, _desc_, _binds_)    \
    this->addKeybindAction(TriggerableAction {                  \
        _name_, _id_, KB_EDITOR_CATEGORY, _sub_,                \
        [](CCNode* context, bool down) -> bool {                \
            _func_;                                             \
            return false;                                       \
        }, _desc_ }, _binds_ );                                 \


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
            GJBaseGameLayer* layer = as<GJBaseGameLayer*>(context);
            if (category == KB_EDITOR_CATEGORY) {
                layer = as<EditorUI*>(context)->m_pEditorLayer;
            }
            if (push) {
                layer->pushButton(0, true);
            } else {
                layer->releaseButton(0, true);
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
            GJBaseGameLayer* layer = as<GJBaseGameLayer*>(context);
            if (category == KB_EDITOR_CATEGORY) {
                layer = as<EditorUI*>(context)->m_pEditorLayer;
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

    ////////////////////////

    { ADD_EDITOR_KB( "Build Mode",
        "gd.edit.build_mode",
        KB_SUBCATEGORY_UI,
        EDIT_ACTION(
            ui->toggleMode(ui->m_pBuildModeBtn);
        ),
        "Toggle the Build Tab",
        BIND( KEY_One, 0 )
    ); }

    { ADD_EDITOR_KB( "Edit Mode",
        "gd.edit.edit_mode",
        KB_SUBCATEGORY_UI,
        EDIT_ACTION(
            ui->toggleMode(ui->m_pEditModeBtn);
        ),
        "Toggle the Edit Tab",
        BIND( KEY_Two, 0 )
    ); }

    { ADD_EDITOR_KB( "Delete Mode",
        "gd.edit.delete_mode",
        KB_SUBCATEGORY_UI,
        EDIT_ACTION(
            ui->toggleMode(ui->m_pDeleteModeBtn);
        ),
        "Toggle the Delete Tab",
        BIND( KEY_Three, 0 )
    ); }

    // this looks ugly as shit, but it's so when i fold it
    // in vs code i can see what keybind it is without
    // needing to unfold
    this->addKeybindAction(KeybindModifier { "Swipe Modifier",
        "gd.edit.swipe_modifier",
        KB_EDITOR_CATEGORY,
        KB_SUBCATEGORY_GLOBAL,
        "When the Swipe Modifier is enabled, clicking anywhere "
        "in the editor enables swipe until the mouse is released"
    }, {{ Keybind::kmShift }});

    this->addKeybindAction(KeybindModifier { "Move Modifier",
        "gd.edit.move_modifier",
        KB_EDITOR_CATEGORY,
        KB_SUBCATEGORY_GLOBAL,
        "If Swipe is enabled, pressing the Move Modifier lets you "
        "move the screen around"
    }, {{ KEY_Space, 0 }});

    this->addKeybindAction(KeybindModifier { "Free Move Modifier",
        "gd.edit.free_move_modifier",
        KB_EDITOR_CATEGORY,
        KB_SUBCATEGORY_GLOBAL,
        "When you press with the mouse, Free Move is enabled "
        "until the mouse button is released"
    }, {{ Keybind::kmControl }});

    this->addKeybindAction(KeybindModifier { "Copy Modifier",
        "gd.edit.duplicate_modifier",
        KB_EDITOR_CATEGORY,
        KB_SUBCATEGORY_GLOBAL,
        "When you press with the mouse, the selected object(s) are"
        "duplicated and Free Move is enabled until the mouse "
        "button is released"
    }, {{ Keybind::kmControl | Keybind::kmAlt }});

    { ADD_EDITOR_KB( "Rotate CCW",
        "gd.edit.rotate_ccw",
        KB_SUBCATEGORY_MODIFY,
        EDIT_ACTION(
            ui->transformObjectCall(kEditCommandRotateCCW);
        ),
        "Rotate Object Counter-Clockwise",
        BIND( KEY_Q, 0 )
    ); }

    { ADD_EDITOR_KB( "Rotate CW",
        "gd.edit.rotate_cw",
        KB_SUBCATEGORY_MODIFY,
        EDIT_ACTION(
            ui->transformObjectCall(kEditCommandRotateCW);
        ),
        "Rotate Object Clockwise",
        BIND( KEY_E, 0 )
    ); }

    { ADD_EDITOR_KB( "Flip X",
        "gd.edit.flip_x",
        KB_SUBCATEGORY_MODIFY,
        EDIT_ACTION(
            ui->transformObjectCall(kEditCommandFlipX);
        ),
        "Flip Object Along the X-axis",
        KBS(Alt, Q)
    ); }

    { ADD_EDITOR_KB( "Flip Y",
        "gd.edit.flip_y",
        KB_SUBCATEGORY_MODIFY,
        EDIT_ACTION(
            ui->transformObjectCall(kEditCommandFlipY);
        ),
        "Flip Object Along the Y-axis",
        KBS(Alt, E)
    ); }

    { ADD_EDITOR_KB( "Delete Selected",
        "gd.edit.delete_selected",
        KB_SUBCATEGORY_MODIFY,
        EDIT_ACTION(
            ui->onDeleteSelected(nullptr);
        ),
        "Delete Selected Object(s)",
        BIND( KEY_Delete, 0 )
    ); }

    { ADD_EDITOR_KB( "Undo",
        "gd.edit.undo",
        KB_SUBCATEGORY_MODIFY,
        EDIT_ACTION(
            ui->undoLastAction(nullptr);
        ),
        "Undo Last Action",
        BIND( KEY_Z, Keybind::kmControl )
    ); }

    { ADD_EDITOR_KB( "Redo",
        "gd.edit.redo",
        KB_SUBCATEGORY_MODIFY,
        EDIT_ACTION(
            ui->redoLastAction(nullptr);
        ),
        "Redo Last Action",
        BIND( KEY_Z, Keybind::kmControl | Keybind::kmShift )
    ); }

    { ADD_EDITOR_KB( "Deselect All",
        "gd.edit.deselect",
        KB_SUBCATEGORY_MODIFY,
        EDIT_ACTION(
            ui->deselectAll();
        ),
        "Deselect All Selected Objects",
        BIND( KEY_D, Keybind::kmAlt )
    ); }

    { ADD_EDITOR_KB( "Copy",
        "gd.edit.copy",
        KB_SUBCATEGORY_MODIFY,
        EDIT_ACTION(
            ui->onCopy(nullptr);
        ),
        "Copy Selected Objects",
        BIND( KEY_C, Keybind::kmControl )
    ); }

    { ADD_EDITOR_KB( "Paste",
        "gd.edit.paste",
        KB_SUBCATEGORY_MODIFY,
        EDIT_ACTION(
            ui->onPaste(nullptr);
        ),
        "Paste Copied Objects",
        BIND( KEY_V, Keybind::kmControl )
    ); }

    { ADD_EDITOR_KB( "Duplicate",
        "gd.edit.copy_and_paste",
        KB_SUBCATEGORY_MODIFY,
        EDIT_ACTION(
            ui->onDuplicate(nullptr);
        ),
        "Copy + Paste Selected Objects",
        BIND( KEY_D, Keybind::kmControl )
    ); }

}
