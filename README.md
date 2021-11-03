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

## About

BetterGD is a specialized mod loader & modding API, meant for creating mods that work together and have strong customizability for the end user.

The BetterGD hooking system is based on the back-end on [lilac](https://github.com/lilac-sdk/core) and the API on [matdash](https://github.com/matcool/mat-dash). Syntactically, it is meant to provide an easy & fun to use syntax that allows creating hooks in a single .cpp-file.

The BetterGD plugin API is designed to enable interoperability between plugins by making it easy for plugins to share their interfaces through inheriting the `BGDPlugin` base class.

The main goal of BetterGD is to simply provide a large set of shared utilities between mods so each mod doesn't have their own instance of things like `SuperMouseManager`, and to minimize the amount of hooks on common targets.

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
            // Plugin description
            this->m_sDescription = "An Awesome Plugin by Me!";
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

 * Hooking backend from [lilac](https://github.com/lilac-sdk/core) by the lilac team
 * Hooking API based on [mat-dash](https://github.com/matcool/mat-dash) by [mat](https://github.com/matcool)
 * Zipping utilities based on [Zipper](https://github.com/sebastiandev/zipper)
