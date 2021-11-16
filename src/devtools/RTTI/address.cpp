#include "address.hpp"
#include "gdrtti.hpp"

constexpr size_t sizeOfDefaultDataType(DataType type) {
    switch (type) {
        case kDataTypeBool:     return sizeof bool;
        case kDataTypeChar:     return sizeof char;
        case kDataTypeInt:      return sizeof int;
        case kDataTypeLong:     return sizeof long;
        case kDataTypeLongLong: return sizeof long long;
        case kDataTypeFloat:    return sizeof float;
        case kDataTypeDouble:   return sizeof double;
        case kDataTypePointer:  return sizeof uintptr_t;
        case kDataTypeVoid:     return 0;
        case kDataTypeUnknown:  return 4;
    }
    return 0;
}

constexpr const char* stringFromDefaultDataType(DataType type) {
    switch (type) {
        case kDataTypeBool:     return "bool";
        case kDataTypeChar:     return "char";
        case kDataTypeInt:      return "int";
        case kDataTypeLong:     return "long";
        case kDataTypeLongLong: return "long long";
        case kDataTypeFloat:    return "float";
        case kDataTypeDouble:   return "double";
        case kDataTypePointer:  return "pointer";
        case kDataTypeVoid:     return "void";
        case kDataTypeUnknown:  return "";
    }
    return "unknown";
}

size_t MemberVariable::calc_size() const {
    return sizeOfDefaultDataType(this->m_type);
}

std::string MemberVariable::type_to_string() const {
    return stringFromDefaultDataType(this->m_type);
}

size_t MixedMemberVarible::calc_size() const {
    size_t res;
    for (auto const& member : this->m_members) {
        res += member.calc_size();
    }
    return res;
}

void AddressManager::deduce_info(address_t addr) {
    if (this->m_info.count(addr))
        return;
    
    AddressInfo info;

    info.m_name = "";
    info.m_type = kDataTypeUnknown;

    auto as_int     = as<int>(addr);
    auto as_float   = union_cast<float>(addr);
    auto as_pointer = as<address_t*>(addr);

    auto& rtti = GDRTTI::get();

    if (-10000.f < as_float && as_float < 10000.f) {
        info.m_type = kDataTypeFloat;
    } else if (-10000 < as_int && as_int < 10000) {
        info.m_type = kDataTypeInt;
    } else if (rtti.valid(as<uintptr_t>(as_pointer))) {
        info.m_type = kDataTypePointer;
        this->deduce_info(*as_pointer);
    }
}

Result<AddressInfo> AddressManager::info_at(address_t addr) {
    if (this->m_info.count(addr)) {
        return Ok<>(this->m_info[addr]);
    }
    return Err<>("No info found");
}
