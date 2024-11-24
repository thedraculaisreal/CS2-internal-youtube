#include <Windows.h>
#include <iostream>
#include "reader.h"
#include "../features/aimbot.h"


void Main(HMODULE hModule) noexcept
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
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
    if (ul_reason_for_call == 1)
    {
        DisableThreadLibraryCalls(hModule);

        const auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Main), hModule, 0, nullptr);
        const auto thread1 = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(read_mem), hModule, 0, nullptr);

        if (thread)
            CloseHandle(thread);
        if (thread1)
            CloseHandle(thread1);
    }

    return TRUE;
}

