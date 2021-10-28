# BetterGD

A Geometry Dash modpack & utility library. Still WIP, but once finished, will contain tons of awesome features & default installed mods that improve the GD experience.

## Features

(Note: Some features have not yet been implemented)

 * Customizable Keybinds
 * Load BGD Plugins

## Plugins

(Note: Some plugins are old mods that have not yet been ported over to BetterGD)

 * BetterEdit
 * GDShare
 * IconSave

## Note

This is **not** a general modding library, nor a general mod loader. BetterGD does not handle hooking nor patching.

The BetterGD plugin API is designed to enable interoperability between plugins by making it easy for plugins to share their interfaces through inheriting the `BGDPlugin` base class.

However, the main goal of BetterGD is to simply provide a large set of shared utilities between mods so each mod doesn't have their own instance of things like SuperMouseManager, and to minimize the amount of hooks on common stuff.

## Example

```cpp
#include <BetterGD.hpp>

class MyPlugin : public bgd::BGDPlugin {
    protected:
        void setup() override {
            // Plugin ID; can be any string,
            // but it is recommended to be
            // formatted as <creator>.<name>
            this->m_sID = "hjfod.my_awesome_plugin";
            // Plugin Name
            this->m_sName = "My Awesome Plugin";
        }
        // A `get` method is required for the
        // `BGD_LOAD` macro, and is useful in
        // general in case you need to access
        // your plugin instance elsewhere in
        // the code.

        // Note that if your plugin interface
        // is available for other mods, this
        // should be defined in a source file
        // instead in order to make sure
        // the other mod doesn't generate its
        // own instance of your plugin.
        BGD_PLUGIN_GET(MyPlugin);
};

BGD_LOAD(MyPlugin);
```

## Credits

[mat-dash](https://github.com/matcool/mat-dash) by [mat](https://github.com/matcool)
