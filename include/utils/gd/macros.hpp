#pragma once

#include "include_gd.hpp"

#define CCARRAY_FOREACH_B_BASE(__array__, __obj__, __type__, __index__) \
    if (__array__ && __array__->count()) \
    for (auto [__index__, __obj__] = std::tuple<unsigned int, __type__> { 0u, nullptr }; \
        (__index__ < __array__->count() && (__obj__ = reinterpret_cast<__type__>(__array__->objectAtIndex(__index__)))); \
        __index__++)

#define CCARRAY_FOREACH_B_TYPE(__array__, __obj__, __type__) \
    CCARRAY_FOREACH_B_BASE(__array__, __obj__, __type__*, ccarray_foreach_b_type_ix##__LINE__)

#define CCARRAY_FOREACH_B(__array__, __obj__) \
    CCARRAY_FOREACH_B_BASE(__array__, __obj__, cocos2d::CCObject*, ccarray_foreach_b_ix_##__LINE__)
