#include <Windows.h>
#include <iostream>

#include "Memory.hh"
#include "Hooks/Hooks.hh"

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD ul_reason_for_call,
					  LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return false;

	DisableThreadLibraryCalls(hModule);

	AllocConsole();
	freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);

	g_pMemory = std::make_unique<Memory_t>();
	Hooks::Bootstrap();

	return true;
}
