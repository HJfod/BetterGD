#pragma once

#include <BGDUtils.hpp>

USE_BGD_NAMESPACE();

enum DataType {
    kDataTypeBool,
    kDataTypeChar,
    kDataTypeInt,
    kDataTypeLong,
    kDataTypeLongLong,
    kDataTypeFloat,
    kDataTypeDouble,
    kDataTypePointer,
    kDataTypeVoid,
    kDataTypeUnknown,
};

constexpr size_t sizeOfDefaultDataType(DataType type);
constexpr const char* stringFromDefaultDataType(DataType type);

struct MemberVariableBase {
    virtual             ~MemberVariableBase()  = default;
    virtual size_t      calc_size()      const = 0;
    virtual std::string type_to_string() const = 0;
};

struct MemberVariable : public MemberVariableBase {
    DataType        m_type;
    bool            m_signed = true;
    bool            m_const  = false;

    size_t      calc_size()      const override;
    std::string type_to_string() const override;

    MemberVariable(
        DataType type
    ) : m_type(type) {}
};

struct PadMemberVariable : public MemberVariableBase {
    size_t m_size;

    inline size_t      calc_size()      const override { return this->m_size; }
    inline std::string type_to_string() const override { return "PAD"; }
};

using MemberList = std::vector<std::tuple<std::string, MemberVariableBase>>;

struct MixedMemberVariable : public MemberVariableBase {
    std::string m_name;
    MemberList m_members;
    size_t calc_size() const override;
};

struct UnionMemberVariable : public MixedMemberVariable {
    inline std::string type_to_string() const override {
        return "union";
    }
};

struct EnumMemberVariable : public MixedMemberVariable {
    DataType                 m_base_type;
    inline std::string type_to_string() const override {
        return "enum";
    }
};

struct StructMemberVariable : public MixedMemberVariable {
    bool                        m_is_class = false;
    inline std::string type_to_string() const override {
        return m_is_class ? "class" : "struct";
    }

    StructMemberVariable(
        std::string const& name,
        MemberList  const& members,
        bool is_class = false
    ) : m_name(name),
        m_members(members),
        m_is_class(is_class) {}
};


static StructMemberVariable MV_CCPoint {
    "CCPoint",
    {
        { "x", kDataTypeFloat },
        { "y", kDataTypeFloat },
    }
};

static StructMemberVariable MV_CCSize {
    "CCSize",
    {
        { "width",  kDataTypeFloat },
        { "height", kDataTypeFloat },
    }
};

static StructMemberVariable MV_CCRect {
    "CCRect",
    {
        { "origin", MV_CCPoint },
        { "size",   MV_CCSize },
    }
};

class AddressInfo {
    std::string m_name;
    MemberVariableBase m_type;
};

class AddressManager {
    protected:
        std::map<address_t, AddressInfo> m_info;

    public:
        void deduce_info(address_t addr);
        Result<AddressInfo> info_at(address_t addr);
};

