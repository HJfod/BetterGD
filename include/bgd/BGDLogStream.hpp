#pragma once

#include <utils/gd/include_gd.hpp>
#include "BGDPlugin.hpp"
#include "BGDError.hpp"

namespace bgd {
    class BGDPlugin;
    class BGDLogStream;

    class BGD_DLL BGDLogMessage {
        protected:
            BGDPlugin* m_plugin = nullptr;
            std::string m_info = "";
            cocos2d::CCObject* m_ccobj = nullptr;
            std::chrono::system_clock::time_point m_time;
            BGDError* m_error = nullptr;

            friend class BGDLogStream;

        public:
            BGDLogMessage(BGDPlugin* plugin, std::string const& info)
                : m_plugin(plugin), m_info(info) {}

            BGDLogMessage(BGDPlugin* plugin, cocos2d::CCObject* obj)
                : m_plugin(plugin), m_ccobj(obj) {}

            BGDLogMessage(BGDError* err)
                : m_plugin(err->plugin), m_error(err) {}
            
            BGDLogMessage() = default;
            
            inline BGDPlugin* getPlugin() const {
                return m_plugin;
            }

            inline std::string getInfo() const {
                return m_info;
            }

            inline cocos2d::CCObject* getCCObj() const {
                return m_ccobj;
            }

            std::string toString() const;
    };

    struct endl_type {
        constexpr endl_type() {}
    };
    constexpr const auto endl = endl_type();

    class BGD_DLL BGDLogStream {
        protected:
            bool m_init = false;
            std::stringstream m_stream;
            BGDLogMessage m_msg;

            void init();

        public:
            BGDLogStream& operator<<(BGDPlugin*);
            BGDLogStream& operator<<(cocos2d::CCObject*);
            BGDLogStream& operator<<(std::string const&);
            BGDLogStream& operator<<(const char*);
            BGDLogStream& operator<<(int);
            BGDLogStream& operator<<(long);
            BGDLogStream& operator<<(float);
            BGDLogStream& operator<<(double);
            BGDLogStream& operator<<(cocos2d::CCPoint const&);
            BGDLogStream& operator<<(cocos2d::CCSize const&);
            BGDLogStream& operator<<(cocos2d::CCRect const&);
            BGDLogStream& operator<<(endl_type);
    };
}
