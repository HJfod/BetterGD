#include <utils/other/platform.hpp>

#ifdef BGD_IS_WIN32

#include <iostream>
#include <sstream>

void bgd::registerFileExtension(
    std::string const& ext,
    FileFlags flags,
    std::function<void(std::vector<std::string> const&)> handler
) {
    // todo
}

bool bgd::copyToClipboard(std::string const& data) {
    if (!OpenClipboard(nullptr))
        return false;
    if (!EmptyClipboard()) {
        CloseClipboard();
        return false;
    }

    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, data.size() + 1);
    
	if (!hg) {
		CloseClipboard();
		return false;
	}

	auto dest = GlobalLock(hg);

	if (!dest) {
		CloseClipboard();
		return false;
	}

	memcpy(dest, data.c_str(), data.size() + 1);

	GlobalUnlock(hg);

	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();

	GlobalFree(hg);

    return true;
}

std::string bgd::readClipboard() {
    if (!OpenClipboard(nullptr))
        return "";
    
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr) {
        CloseClipboard();
        return "";
    }

    char * pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        return "";
    }

    std::string text(pszText);

    GlobalUnlock(hData);
    CloseClipboard();

    return text;
}

bool bgd::loadConsole() {
    if (AllocConsole() == 0)
        return false;
    // redirect console output
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);

    return true;
}

bool bgd::unloadConsole() {
    fclose(stdin);
    fclose(stdout);
    return FreeConsole();
}

void bgd::bufferConsoleInput() {
    std::string inp;
    getline(std::cin, inp);
    std::string inpa;
    std::stringstream ss(inp);
    std::vector<std::string> args;

    while (ss >> inpa) args.push_back(inpa);
    ss.clear();

    if (inp != "e") bufferConsoleInput();
}

#endif
