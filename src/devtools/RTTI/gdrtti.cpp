#include "gdrtti.hpp"

GDRTTI::GDRTTI() {
    this->m_process_info.push_back({
        "Geometry Dash",
        GetCurrentProcessId(),
        GetCurrentProcess()
    });
    this->m_selected_process = 0;
}

GDRTTI::~GDRTTI() {
    this->close_old_handle();
}

GDRTTI& GDRTTI::get() {
    static auto g = new GDRTTI;
    return *g;
}

std::string GDRTTI::read_rtti(void* pointer) {
    return this->RTTI::read_rtti(reinterpret_cast<uint32_t*>(pointer)[0]);
}
