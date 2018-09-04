#include "Interfaces.h"
#include "Hooks.h"
#include "Render.h"
#include "SDK.h"
#include "recvproxy.h"
#include "Misc.h"
#include "Global.h"
#include "SetupColors.h"
#include <TlHelp32.h>

bool unload;

DWORD GetProcessId(LPCTSTR name)
{
	PROCESSENTRY32 pe32;
	HANDLE snapshot = NULL;
	DWORD pid = 0;

	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot != INVALID_HANDLE_VALUE) {
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(snapshot, &pe32)) {
			do {
				std::string sName = pe32.szExeFile;
				std::transform(sName.begin(), sName.end(), sName.begin(), ::tolower);

				if (!lstrcmp(sName.c_str(), name)) {
					pid = pe32.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshot, &pe32));
		}
		CloseHandle(snapshot);
	}
	return pid;
}

bool on_dll_attach(void* base)
{
	Interfaces::Initialise();
	g_Netvars->GetNetvars();
	g_Render->SetupFonts();
	hooks::initialize();
	SetupColors();
	NetvarHook();

	while (unload == false)
	{
		Sleep(1000);
	}

	UnloadProxy();
	hooks::cleanup();

	Sleep(2000);
	FreeLibraryAndExitThread((HMODULE)base, 0);
}

bool on_dll_detach()
{
	UnloadProxy();
	hooks::cleanup();
	return 1;
}

BOOL WINAPI DllMain(
	_In_      HINSTANCE hinstDll,
	_In_      DWORD     fdwReason,
	_In_opt_	LPVOID    lpvReserved
)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)on_dll_attach, hinstDll, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		if (lpvReserved == nullptr)
			return on_dll_detach();
	}
	return TRUE;
}












































































































































































































































































































































































































































































































































































