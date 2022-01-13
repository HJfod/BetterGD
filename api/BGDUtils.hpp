#pragma once

#include <lilac.hpp>

USE_LILAC_NAMESPACE();

namespace bgd {}

#define USE_BGD_NAMESPACE() using namespace bgd;

#ifdef _EXPORTING_BGD
    #define BGD_DLL     __declspec(dllexport)
#else
    #define BGD_DLL     __declspec(dllimport)
#endif

#pragma warning(disable: 4251)
