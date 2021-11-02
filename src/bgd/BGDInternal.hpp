#pragma once

#include <BGDUtils.hpp>

class BGDInternal {
    private:
    public:
        void addResourceSearchPaths();
        void setup();

        static BGDInternal* get();
};
