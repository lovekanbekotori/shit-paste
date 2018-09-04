#include "HookIncludes.h"
#include "HooksList.h"
#include "Listener.h"

namespace hooks
{
	vfunc_hook panel;
	vfunc_hook client;
	vfunc_hook clientmode;
	vfunc_hook modelrender;
	vfunc_hook prediction;
	vfunc_hook surface;
	vfunc_hook eventmanager;
	vfunc_hook viewrender;
	vfunc_hook engine;
	vfunc_hook d3d;
	vfunc_hook sv_cheats;

	void initialize()
	{
		client.setup(Interfaces::g_CHLClient);
		client.hook_index(37, hkFrameStageNotify);
		client.hook_index(22, hkCreateMove);

		clientmode.setup(Interfaces::g_ClientMode);
		clientmode.hook_index(18, hkOverrideView);
		clientmode.hook_index(35, hkGetViewModelFOV);
		clientmode.hook_index(44, hkDoPostScreenSpaceEffects);

		panel.setup(Interfaces::g_Panel);
		panel.hook_index(41, hkPaintTraverse);

		modelrender.setup(Interfaces::g_ModelRender);
		modelrender.hook_index(21, hkDrawModelExecute);
		//		modelrender.hook_index(84, hkFindMaterial);

		viewrender.setup(Interfaces::g_RenderView);
		viewrender.hook_index(9, Hooked_SceneEnd);

		surface.setup(Interfaces::g_Surface);
		surface.hook_index(82, hkPlaySound);

		engine.setup(Interfaces::g_Engine);
		engine.hook_index(27, IsConnected);

		initializeskins();

		window = FindWindow("Valve001", NULL);
		G::OldWindow = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)handlers::Hooked_WndProc);


		if (Interfaces::g_D3DDevice9)
		{
			d3d.setup(Interfaces::g_D3DDevice9);
			d3d.hook_index(16, handlers::Hooked_Reset);
			d3d.hook_index(42, handlers::Hooked_EndScene);
		}
		item_purchase::singleton()->initialize();
	}

	void cleanup()
	{
		item_purchase::singleton()->remove();
		client.unhook_all();
		prediction.unhook_all();
		clientmode.unhook_all();
		panel.unhook_all();
		modelrender.unhook_all();
		surface.unhook_all();
		//EventManager.Unhook();
		viewrender.unhook_all();
		//d3d.Unhook();
		sv_cheats.unhook_all();
		engine.unhook_all();
	}
}




































































































































































































































































































































































































































































































































































