#pragma once

#include "rtti.hpp"
#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

class GDRTTI : public RTTI {
    protected:
        GDRTTI();
        ~GDRTTI();

    public:
        static GDRTTI& get();
        std::string read_rtti(void*);

        GDRTTI(GDRTTI const&) = delete;
	    GDRTTI& operator=( const GDRTTI& ) = delete;
};
