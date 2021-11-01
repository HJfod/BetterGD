#include <BetterGD.hpp>

#ifdef _EXPORTING_MYPLUGIN
#define MYPLUGIN_DLL __declspec(dllexport)
#else
#define MYPLUGIN_DLL __declspec(dllimport)
#endif

class MYPLUGIN_DLL MyPlugin : public bgd::BGDPlugin {
    protected:
        void setup() override;

    public:
        void logText(std::string const& str);

        static MyPlugin* get();
};
