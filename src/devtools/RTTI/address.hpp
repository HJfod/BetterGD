#pragma once

#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

enum DataType {
    kDataTypeBool,
    kDataTypeChar,
    kDataTypeInt,
    kDataTypeFloat,
    kDataTypeLong,
    kDataTypeDouble,
    kDataTypePointer,
    kDataTypeStruct,
};

struct datatype {
    bool    is_const    = false;
    bool    is_signed   = true;
    size_t  size        = 0;

    virtual std::string to_string() const = 0;
    virtual ~datatype() = default;
};

#define DEFINE_NORMAL_DATATYPE(type, type2)             \
    struct dt_##type : public datatype {                \
        inline std::string to_string() const override { \
            return #type #type2;                        \
        }                                               \
        dt_##type() : size(sizeof type type2) {}        \
    };                                                  \

DEFINE_NORMAL_DATATYPE(bool);
DEFINE_NORMAL_DATATYPE(int);
DEFINE_NORMAL_DATATYPE(long);
DEFINE_NORMAL_DATATYPE(long, long);
DEFINE_NORMAL_DATATYPE(char);
DEFINE_NORMAL_DATATYPE(float);
DEFINE_NORMAL_DATATYPE(double);

struct dt_void : public datatype {
    inline std::string to_string() const override { return "void"; }
};

struct MixedDataType {
    std::vector<DataType> m_members;
};

class AddressInfo {
    std::string m_name;
    DataType    m_type; 
};

class AddressManager {
    protected:
        std::map<address_t, AddressInfo> m_info;

    public:
        void load_info(address_t addr);
        Result<AddressInfo> info_at(address_t addr);
};

