#pragma once

#include <BGDMacros.hpp>
#include <string_view>

namespace bgd {
    // credit to matcool
    // https://github.com/matcool

    struct no_result {};

    template <class T = no_result, class E = std::string_view>
    class Result {
    protected:
        bool success;
        union {
            T my_value;
            E error_msg;
        };
        Result(const T value) : success(true), my_value(value) {}
        Result(const E error, int dummy) : success(false), error_msg(error) {}

    public:
        ~Result() {
            if (success) {
                if (std::is_destructible<T>::value) {
                    my_value.~T();
                }
            } else {
                if (std::is_destructible<E>::value) {
                    error_msg.~E();
                }
            }
        }
        Result(Result<T, E> const& other) {
            if (other.success) {
                this->success = true;
                this->my_value = other.my_value;
            } else {
                this->success = false;
                this->error_msg = other.error_msg;
            }
        }
        template<class T2, class E2>
        Result(Result<T2, E2> const& other) {
            if (other) {
                this->success = true;
                if constexpr (!std::is_same<T, no_result>::value) {
                    this->my_value = other.value();
                }
            } else {
                this->success = false;
                this->error_msg = other.error();
            }
        }

        bool is_error() const { return !success; }
        bool is_value() const { return success; }
        auto value() const { return my_value; }
        auto error() const { return error_msg; }

        operator bool() const { return this->success; }

        static auto ok(const T value) { return Result<T>(value); }
        static auto err(E error) { return Result<T>(error, 0); }
    };

    template <class T = no_result>
    Result<T> Ok(T value = T()) {
        return Result<T>::ok(value);
    }

    template <class E = std::string_view>
    struct Err {
        const E _value;
        Err(const E value) : _value(value) {}
        template <class T>
        operator Result<T>() const {
            return Result<T>::err(_value);
        }
    };
}
