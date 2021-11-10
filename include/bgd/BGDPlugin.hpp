#pragma once

#include "BGDMacros.hpp"
#include "BGDError.hpp"
#include "BGDLog.hpp"
#include "BGDHook.hpp"
#include "../utils/other/ext.hpp"
#include "../matdash/matdash.hpp"
#include <string>
#include <vector>

class BGDInternal;
class DevTools;

namespace bgd {
    class BGDLoader;
    class BGDSaveManager;
    class BGDLogStream;

    struct BGDPlatformInfo;

    #pragma warning(disable: 4251) // i will use unordered_map and
                                   // no amount of compiler warnings
                                   // can stop me >:)
                                   
    class BGD_DLL BGDPluginBase {
        protected:
            std::string_view m_sPath;
            BGDPlatformInfo* m_pInfo;
            std::vector<BGDSaveManager*> m_vSaveManagers;
            std::vector<BGDHook*> m_vHooks;

            bgd::Result<BGDHook*> addHookBase(
                void* addr,
                void* detour,
                BGDHook* hook = nullptr
            );
            bgd::Result<BGDHook*> addHookBase(BGDHook* hook);
    };

    class BGD_DLL BGDPlugin : BGDPluginBase {
        protected:
            std::string_view m_sID;
            std::string_view m_sName;
            std::string_view m_sDeveloper;
            std::string_view m_sDescription;
            std::string_view m_sCredits;

            void platformCleanup();

            virtual void setup() = 0;
            virtual void enable();
            virtual void disable();
            virtual void saveData();
            virtual void loadData();

            void registerSaveManager(BGDSaveManager*);
            void unregisterSaveManager(BGDSaveManager*);

            friend class BGDLoader;
            friend class BGDSaveManager;
            friend class BGDInternal;
            friend class DevTools;

            bgd::Result<BGDHook*> addHookInternal(
                void* addr,
                void* detour,
                void** trampoline
            );

        public:
            std::string_view getID()         const;
            std::string_view getName()       const;
            std::string_view getDeveloper()  const;
            std::string_view getCredits()    const;
            std::string_view getPath()       const;
            std::string_view getDescription()const;

            BGDLogStream& log();
            void throwError(
                std::string_view const& info,
                std::string_view const& fullDescription,
                BGDSeverity severity,
                BGDErrorType type
            );

            BGDPlugin();
            virtual ~BGDPlugin();

            std::vector<BGDHook*> getHooks() const;

            template <auto Func, typename CallConv = bgd::hook::Optcall>
            bgd::Result<BGDHook*> addHook(uintptr_t address) {
                using namespace bgd::hook;
                const auto addr = reinterpret_cast<void*>(address);
                if constexpr (std::is_same_v<CallConv, Optcall>) {
                    if constexpr (std::is_member_function_pointer_v<decltype(Func)>) {
                        return this->addHookInternal(
                            addr,
                            &optcall<RemoveThiscall<MemberToFn<decltype(Func)>::type>::type>::wrap<WrapMemberCall<Func>::wrap>,
                            reinterpret_cast<void**>(&Orig<Func, Optcall>::orig)
                        );
                    } else {
                        return this->addHookInternal(
                            addr,
                            &optcall<decltype(Func)>::wrap<Func>,
                            reinterpret_cast<void**>(&Orig<Func, Optcall>::orig)
                        );
                    }
                } else if constexpr (std::is_same_v<CallConv, Thiscall>) {
                    return this->addHookInternal(
                        addr,
                        &thiscall<decltype(Func)>::wrap<Func>,
                        reinterpret_cast<void**>(&Orig<Func, Thiscall>::orig)
                    );
                } else if constexpr (std::is_same_v<CallConv, Optfastcall>) {
                    return this->addHookInternal(
                        addr,
                        &optfastcall<decltype(Func)>::wrap<Func>,
                        reinterpret_cast<void**>(&Orig<Func, CallConv>::orig)
                    );
                } else {
                    static_assert(std::false_type::value, "Invalid calling convention");
                    return bgd::Err<>("Invalid calling convention");
                }
                return bgd::Err<>("Unknown Error");
            }

            template <auto Func, typename CallConv = bgd::hook::Optcall>
            bgd::Result<BGDHook*> addHook(void* address) {
                return this->addHook<Func, CallConv>(reinterpret_cast<uintptr_t>(address));
            }
    
            bgd::Result<> enableHook(BGDHook* hook);
            bgd::Result<> disableHook(BGDHook* hook);
            bgd::Result<> removeHook(BGDHook* hook);
    };

    typedef bgd::BGDPlugin* (__stdcall* bgd_load_type)();
}
