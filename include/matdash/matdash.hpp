#pragma once

namespace matdash {
    #include "template-utils.hpp"

    void __mat_dash_add_hook(void* addr, void* detour, void** trampoline);

    template <auto func, typename CallConv = Optcall>
    void add_hook(uintptr_t address) {
        const auto addr = reinterpret_cast<void*>(address);
        if constexpr (std::is_same_v<CallConv, Optcall>) {
            if constexpr (std::is_member_function_pointer_v<decltype(func)>) {
                __mat_dash_add_hook(addr,
                    &optcall<RemoveThiscall<MemberToFn<decltype(func)>::type>::type>::wrap<WrapMemberCall<func>::wrap>,
                    reinterpret_cast<void**>(&Orig<func, Optcall>::orig)
                );
            } else {
                __mat_dash_add_hook(addr,
                    &optcall<decltype(func)>::wrap<func>,
                    reinterpret_cast<void**>(&Orig<func, Optcall>::orig)
                );
            }
        } else if constexpr (std::is_same_v<CallConv, Thiscall>) {
            __mat_dash_add_hook(addr,
                &thiscall<decltype(func)>::wrap<func>,
                reinterpret_cast<void**>(&Orig<func, Thiscall>::orig)
            );
        } else if constexpr (std::is_same_v<CallConv, Optfastcall>) {
            __mat_dash_add_hook(addr,
                &optfastcall<decltype(func)>::wrap<func>,
                reinterpret_cast<void**>(&Orig<func, CallConv>::orig)
            );
        } else {
            static_assert(std::false_type::value, "Invalid calling convention");
        }
    }

    template <auto func, typename CallConv = Optcall>
    void add_hook(void* address) {
        add_hook<func, CallConv>(reinterpret_cast<uintptr_t>(address));
    }

    template <typename R, typename T, typename... Args>
    void* member_addr(R(T::* func)(Args...)) {
        return reinterpret_cast<void*&>(func);
    }
}
