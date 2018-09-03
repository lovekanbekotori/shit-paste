	#pragma once
	#include "HookIncludes.h"
	typedef void(__thiscall* play_sound_t)(void*, const char*);

	void __stdcall hkPlaySound(const char* szFileName)
	{
		static auto ofunc = hooks::surface.get_original<play_sound_t>(82);
		//Call original PlaySound
		ofunc(Interfaces::g_Surface, szFileName);

		if (Interfaces::g_Engine->IsInGame()) return;

		if (g_Options.Misc.AutoAccept)
		{
			static auto SetLocalPlayerReadyFn = reinterpret_cast<bool(__stdcall*)(const char*)>(U::pattern_scan("client_panorama.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));
			if (SetLocalPlayerReadyFn)
				SetLocalPlayerReadyFn("deferred");
		}
	}