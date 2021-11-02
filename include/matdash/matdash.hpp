#pragma once

namespace bgd::hook {
    #include "../utils/other/ext.hpp"
    #include <BGDMacros.hpp>
    #include <BGDPlugin.hpp>
    #include "template-utils.hpp"

    bgd::Result<> BGD_DLL __mat_dash_add_hook(void* addr, void* detour, void** trampoline, bgd::BGDPlugin* owner);

    template <auto func, typename CallConv = Optcall>
    bgd::Result<> add_hook(uintptr_t address, bgd::BGDPlugin* owner) {
        const auto addr = reinterpret_cast<void*>(address);
        if constexpr (std::is_same_v<CallConv, Optcall>) {
            if constexpr (std::is_member_function_pointer_v<decltype(func)>) {
                return __mat_dash_add_hook(addr,
                    &optcall<RemoveThiscall<MemberToFn<decltype(func)>::type>::type>::wrap<WrapMemberCall<func>::wrap>,
                    reinterpret_cast<void**>(&Orig<func, Optcall>::orig),
                    owner
                );
            } else {
                return __mat_dash_add_hook(addr,
                    &optcall<decltype(func)>::wrap<func>,
                    reinterpret_cast<void**>(&Orig<func, Optcall>::orig),
                    owner
                );
            }
        } else if constexpr (std::is_same_v<CallConv, Thiscall>) {
            return __mat_dash_add_hook(addr,
                &thiscall<decltype(func)>::wrap<func>,
                reinterpret_cast<void**>(&Orig<func, Thiscall>::orig),
                owner
            );
        } else if constexpr (std::is_same_v<CallConv, Optfastcall>) {
            return __mat_dash_add_hook(addr,
                &optfastcall<decltype(func)>::wrap<func>,
                reinterpret_cast<void**>(&Orig<func, CallConv>::orig),
                owner
            );
        } else {
            static_assert(std::false_type::value, "Invalid calling convention");
            return bgd::Err<>("Invalid calling convention");
        }
        return bgd::Err<>("Unknown Error");
    }

    template <auto func, typename CallConv = Optcall>
    bgd::Result<> add_hook(void* address, bgd::BGDPlugin* owner) {
        return add_hook<func, CallConv>(reinterpret_cast<uintptr_t>(address), owner);
    }

    template <typename R, typename T, typename... Args>
    void* member_addr(R(T::* func)(Args...)) {
        return reinterpret_cast<void*&>(func);
    }
}
