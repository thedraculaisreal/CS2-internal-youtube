#include <Windows.h>
#include <iostream>
#include "reader.h"
#include "../features/aimbot.h"
#include "../hook/hook.h"


void Main(HMODULE hModule) noexcept
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    Hook::init_hook();
    Sleep(1000);
    Aimbot::do_aimbot();

    FreeLibraryAndExitThread(hModule, 0);
}

void read_mem(HMODULE hModule) noexcept
{
    read.init();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Main), hModule, 0, nullptr);
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(read_mem), hModule, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        Hook::unhook_present();
        break;
    }
    return TRUE;
}

