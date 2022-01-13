#pragma once

#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

inline const char* getNodeName(CCObject* node) {
    return typeid(*node).name() + 6;
}

class BGDInternal {
    protected:
        void loadKeybinds();
        
    public:
        void addResourceSearchPaths();
        void setup();
        static bool isFileInSearchPaths(const char*);

        bool loadHooks();

        static BGDInternal* get();
};
