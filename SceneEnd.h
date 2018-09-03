#include "HookIncludes.h"
#include "Global.h"

typedef void(__thiscall *SceneEnd_t)(void *pEcx);

void __fastcall Hooked_SceneEnd(void *pEcx, void *pEdx) {
	static auto ofunc = hooks::viewrender.get_original<SceneEnd_t>(9);
	ofunc(pEcx);
}