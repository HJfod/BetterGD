#pragma once

namespace bgd::hook {
    #include "template-utils.hpp"

    template <typename R, typename T, typename... Args>
    void* member_addr(R(T::* func)(Args...)) {
        return reinterpret_cast<void*&>(func);
    }
}
