#pragma once
#include <type_traits>

template <class F>
struct optcall;

template <typename R, typename... Args>
struct optcall<R(*)(Args...)> : public optcall<R(Args...)> {};

template <class F>
struct optfastcall;

template <typename R, typename... Args>
struct optfastcall<R(*)(Args...)> : public optfastcall<R(Args...)> {};

template<class F>
struct WrapperOptcall;

template <typename R, typename... Args>
struct WrapperOptcall<R(*)(Args...)> : public WrapperOptcall<R(__thiscall*)(Args...)> {};

template<class F>
struct WrapperOptfastcall;

template <typename R, typename... Args>
struct WrapperOptfastcall<R(*)(Args...)> : public WrapperOptfastcall<R(__fastcall*)(Args...)> {};

#include "gen.hpp"

template <auto func>
struct WrapMemberCall {
    template <typename T, typename... Args>
    static auto wrap(T* instance, Args... args) {
        return (instance->*func)(args...);
    }
};

template <typename F>
struct MemberToFn;

template <typename R, typename T, typename... Args>
struct MemberToFn<R(T::*)(Args...)> {
    using type = typename R(__thiscall*)(T*, Args...);
};

template <typename F>
struct RemoveThiscall;

template <typename R, typename... Args>
struct RemoveThiscall<R(__thiscall*)(Args...)> {
    using type = typename R(Args...);
};

template <typename F>
struct AddThiscall;

template <typename R, typename... Args>
struct AddThiscall<R(Args...)> {
    using type = typename R(__thiscall*)(Args...);
};

template <typename R, typename... Args>
struct AddThiscall<R(*)(Args...)> {
    using type = typename R(__thiscall*)(Args...);
};

// to be used in template args
struct Optcall;
struct Thiscall;
struct Optfastcall;

template <auto func, typename CallConv, typename enable = void>
struct Orig {
    inline static WrapperOptcall<decltype(func)> orig;
};

template <auto func, typename CallConv>
struct Orig<func, CallConv, std::enable_if_t<std::is_member_function_pointer<decltype(func)>::value> > {
    inline static WrapperOptcall<typename MemberToFn<decltype(func)>::type> orig;
};

template <auto func>
struct Orig<func, Thiscall, void> {
    inline static typename AddThiscall<decltype(func)>::type orig;
};

template <auto func>
struct Orig<func, Optfastcall, void> {
    inline static WrapperOptfastcall<decltype(func)> orig;
};

template <auto func, typename CallConv = Optcall, typename... Args>
auto orig(Args... args) {
    return Orig<func, CallConv>::orig(args...);
}

template <typename F>
struct thiscall;

template<class R, class... Args>
struct thiscall<R(*)(Args...)> : public thiscall<R(Args...)> {};

template<class R, typename T, class... Args>
struct thiscall<R(T, Args...)> {
    using F = R(T, Args...);
    template <F func>
    static R __fastcall wrap(T self, void*, Args... args) { return func(self, args...); }
};