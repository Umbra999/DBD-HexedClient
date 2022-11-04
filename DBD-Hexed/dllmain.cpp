#include <cstdio>
#include "Wrapper/Utils.hpp"
#include "Core/Patching.hpp"
#include "Core/Handler.hpp"
#include "Core/GUI.hpp"

void Initialize()
{
    AllocConsole();
    SetConsoleOutputCP(65001);
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    std::string title = "HEXED | CONSOLE | " + Utils::RandomString(16);
    SetConsoleTitleA(title.c_str());

    Patching::ApplyPatches();
    GUI::KieroInit();
    Handler::SDKInit();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            Initialize();
            break;
    }
    return TRUE;
}

