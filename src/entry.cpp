#include <BetterGD.hpp>

DWORD WINAPI load_thread(LPVOID hModule) {
    bgd::BGDLoader::get()->setup();
    BGD_PLATFORM_CONSOLE(
        bgd::bufferConsoleInput();
        bgd::unloadConsole();
    )
    return 0;
}

BOOL WINAPI DllMain(
    HINSTANCE hModule,
    DWORD fdwReason,
    LPVOID lpReserved
) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            HANDLE _ = CreateThread(0, 0, load_thread, hModule, 0, nullptr);
            if (_) CloseHandle(_);
            break;
    }
    return TRUE;
}
