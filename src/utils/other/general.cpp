#include <utils/other/general.hpp>

std::string bgd::timePointAsString(const std::chrono::system_clock::time_point& tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    char buf[128];
    ctime_s(buf, sizeof buf, &t);
    std::string res = buf;
    return res.substr(0, res.size() - 1);
}
