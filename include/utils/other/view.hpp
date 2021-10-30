#pragma once

#include "vector.hpp"

namespace bgd {
    template<typename T>
    struct vector_combine {
        std::vector<T> m_vec;
        vector_combine(std::vector<T> const& vec) {
            m_vec = vec;
        }
        vector_combine& operator&(std::vector<T> const& another) {
            m_vec.insert(m_vec.end(), another.begin(), another.end());
        }
        std::vector<T>& operator&(int) {
            return m_vec;
        }
    };
}
