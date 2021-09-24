#include <Windows.h>

#include <iostream>

#include "Hooks/Hooks.hh"

BOOL APIENTRY
DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call != DLL_PROCESS_ATTACH)
        return false;

    DisableThreadLibraryCalls(hModule);

    Hooks::Bootstrap();

    return true;
}
