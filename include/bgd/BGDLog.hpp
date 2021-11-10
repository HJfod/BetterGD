#pragma once

#include <utils/gd/include_gd.hpp>
#include "BGDPlugin.hpp"
#include "BGDError.hpp"

namespace bgd {
    class BGDPlugin;
    class BGDLogStream;

    class BGD_DLL BGDLog {
        protected:
            BGDPlugin* m_pPlugin;
            std::chrono::system_clock::time_point m_obTime;

            friend class BGDLogStream;

        public:
            virtual std::string toString(bool logTime) const = 0;
            virtual std::string toShortString() const = 0;

            virtual ~BGDLog();

            BGDLog() :
                m_pPlugin(nullptr),
                m_obTime(std::chrono::system_clock::now()) {}
            BGDLog(BGDPlugin* plugin) :
                m_pPlugin(plugin),
                m_obTime(std::chrono::system_clock::now()) {}
    };

    class BGD_DLL BGDLogPlugin : public BGDLog {
        protected:
            friend class BGDLogStream;
        
        public:
            BGDLogPlugin(BGDPlugin* plugin) :
                BGDLog(plugin) {}

            std::string toString(bool logTime = false) const override;
            std::string toShortString() const override;
    };

    class BGD_DLL BGDLogError : public BGDLog {
        protected:
            std::string m_sInfo;
            std::string m_sDescription;
            BGDSeverity m_eSeverity;
            BGDErrorType m_eType;

            friend class BGDLogStream;

        public:
            std::string toString(bool logTime = true) const override;
            std::string toShortString() const override;

            inline BGDSeverity getSeverity() const {
                return m_eSeverity;
            }
            inline BGDErrorType getErrorType() const {
                return m_eType;
            }
            inline std::string const& getDescription() const {
                return m_sDescription;
            }
            inline std::string const& getInfo() const {
                return m_sInfo;
            }

            BGDLogError() = default;
            BGDLogError(
                std::string const& info,
                std::string const& fullDescription,
                BGDSeverity severity,
                BGDErrorType type,
                BGDPlugin* plugin
            ) : m_sInfo(info),
                m_sDescription(fullDescription),
                m_eSeverity(severity),
                m_eType(type),
                BGDLog(plugin) {}
    };

    class BGD_DLL BGDLogCCObject : public BGDLog {
        protected:
            cocos2d::CCObject* m_pObj;
        
            friend class BGDLogStream;

        public:
            BGDLogCCObject() = default;
            BGDLogCCObject(
                cocos2d::CCObject* obj,
                BGDPlugin* plugin
            ) : m_pObj(obj),
                BGDLog(plugin) {
                obj->retain();
            }
            ~BGDLogCCObject();
            
            std::string toString(bool logTime = false) const override;
            std::string toShortString() const override;
    };

    class BGD_DLL BGDLogMessage : public BGDLog {
        protected:
            std::string m_sMessage;

            friend class BGDLogStream;
        
        public:
            BGDLogMessage() = default;
            BGDLogMessage(
                std::string const& msg,
                BGDPlugin* plugin
            ) : BGDLog(plugin),
                m_sMessage(msg) {}

            std::string toString(bool logTime = true) const override;
            std::string toShortString() const override;
    };

    class BGD_DLL BGDLogMany : public BGDLog {
        protected:
            std::vector<BGDLog*> m_vLogs;

            friend class BGDLogStream;

        public:
            BGDLogMany(
                std::vector<BGDLog*> const& logs,
                BGDPlugin* plugin
            ) : BGDLog(plugin),
                m_vLogs(logs) {}
            virtual ~BGDLogMany();
            
            inline void add(BGDLog* log) {
                this->m_vLogs.push_back(log);
            }

            std::string toString(bool logTime = false) const override;
            std::string toShortString() const override;
    };

    struct endl_type {
        constexpr endl_type() {}
    };
    constexpr const auto endl = endl_type();

    class BGD_DLL BGDLogStream {
        protected:
            BGDLog* m_pLog = nullptr;
            std::stringstream m_sStream;

            void init();

        public:
            BGDLogStream& operator<<(BGDPlugin*);
            BGDLogStream& operator<<(void*);
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

            ~BGDLogStream();
    };
}
