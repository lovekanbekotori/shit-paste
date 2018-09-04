#pragma once
#include "singleton.hpp"
#include "MiscClasses.h"
#include "Interfaces.h"
#include "Interface.h"
#include "Sounds.h"
#include "DamageIndicator.h"

char* HitgroupToName(int hitgroup)
{
	switch (hitgroup)
	{
	case 1:
		return "head";
	case 6:
		return "arm";
	case 7:
		return "leg";
	case 3:
		return "stomach";
	default:
		return "body";
	}
}


std::vector<cbullet_tracer_info> logs;

#pragma comment(lib, "winmm.lib")
class item_purchase

	: public singleton<item_purchase>
{
	class item_purchase_listener
		: public IGameEventListener2
	{
	public:
		void start()
		{
			Interfaces::g_EventManager->AddListener(this, "item_purchase", false);
			Interfaces::g_EventManager->AddListener(this, "player_hurt", false);
			Interfaces::g_EventManager->AddListener(this, "bullet_impact", false);
		}
		void stop()
		{
			Interfaces::g_EventManager->RemoveListener(this);
		}
		void FireGameEvent(IGameEvent *event) override
		{
			singleton()->on_fire_event(event);
		}
		int GetEventDebugID(void) override
		{
			return 42 /*0x2A*/;
		}
	};

public:

	static item_purchase* singleton()
	{
		static item_purchase* instance = new item_purchase;
		return instance;
	}

	void initialize()
	{
		listener.start();
	}

	void remove()
	{
		listener.stop();
	}

	void on_fire_event(IGameEvent* event)
	{

		if (!strcmp(event->GetName(), "item_purchase"))
		{

			C_BaseEntity* local = Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer());
			auto buyer = event->GetInt("userid");
			std::string gun = event->GetString("weapon");

			if (strstr(gun.c_str(), "molotov")
				|| strstr(gun.c_str(), "nade")
				|| strstr(gun.c_str(), "kevlar")
				|| strstr(gun.c_str(), "decoy")
				|| strstr(gun.c_str(), "suit")
				|| strstr(gun.c_str(), "flash")
				|| strstr(gun.c_str(), "vest")
				|| strstr(gun.c_str(), "cutter")
				|| strstr(gun.c_str(), "defuse")
				)  return;

			auto player_index = Interfaces::g_Engine->GetPlayerForUserID(buyer);
			C_BaseEntity* player = (C_BaseEntity*)Interfaces::g_EntityList->GetClientEntity(player_index);
			player_info_t pinfo;

			if (player && local && Interfaces::g_Engine->GetPlayerInfo(player_index, &pinfo))
			{

				if (g_Options.Misc.eventlogs2)
				{
					gun.erase(gun.find("weapon_"), 7);
					if (player->GetTeamNum() != local->GetTeamNum())
					{
						/*						if (g_ChatElement)
						{
						g_ChatElement->ChatPrintf(0, 0, " ""\x04""[ENEMY]  %s bought %s\n", pinfo.name, gun.c_str());
						} */
						G::Msg("[shitpaste.cc] %s bought %s\n", pinfo.name, gun.c_str());
					}
				}
			}

		}
		if (!strcmp(event->GetName(), "player_hurt"))
		{
			if (Interfaces::g_Engine->GetPlayerForUserID(event->GetInt("attacker")) == Interfaces::g_Engine->GetLocalPlayer() &&
				Interfaces::g_Engine->GetPlayerForUserID(event->GetInt("userid")) != Interfaces::g_Engine->GetLocalPlayer())
			{

			}

			auto spicy = event->GetInt("userid");
			auto jalapeno = event->GetInt("attacker");
			int dmg = event->GetInt("dmg_health");

			C_BaseEntity* local = Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer());
			auto spicy_index = Interfaces::g_Engine->GetPlayerForUserID(spicy);
			auto jalapeno_index = Interfaces::g_Engine->GetPlayerForUserID(jalapeno);
			C_BaseEntity* spicy_ = (C_BaseEntity*)Interfaces::g_EntityList->GetClientEntity(spicy_index);
			C_BaseEntity* jalapeno_ = (C_BaseEntity*)Interfaces::g_EntityList->GetClientEntity(jalapeno_index);

			if (jalapeno_ == local)
			{
				if (g_Options.Visuals.Active)
				{
					switch (g_Options.Visuals.Hitsound)
					{
					case 0: break;
					case 1: PlaySoundA(Default, NULL, SND_ASYNC | SND_MEMORY); break; // COD
					case 2:	PlaySoundA(Roblox, NULL, SND_ASYNC | SND_MEMORY); break; // Anime
					}
				}
				if (g_Options.Visuals.Hitmarker && g_Options.Visuals.Active)
				{
					G::hitmarkeralpha = 1.f;
				}
			}

			if (g_Options.Visuals.Damageindicator && g_Options.Visuals.Active)
			{
				DamageIndicator_t DmgIndicator;
				DmgIndicator.iDamage = dmg;
				DmgIndicator.Player = spicy_;
				DmgIndicator.flEraseTime = local->GetTickBase() * Interfaces::g_Globals->interval_per_tick + 3.f;
				DmgIndicator.bInitialized = false;
				damage_indicators.data.push_back(DmgIndicator);
			}
		}
		if (!strcmp(event->GetName(), "bullet_impact"))
		{
			C_BaseEntity* LocalPlayer = Interfaces::g_EntityList->GetClientEntity(Interfaces::g_Engine->GetLocalPlayer());

			if (LocalPlayer)
			{
				auto index = Interfaces::g_Engine->GetPlayerForUserID(event->GetInt("userid"));

				if (index != Interfaces::g_Engine->GetLocalPlayer())
					return;

				auto local = static_cast<C_BaseEntity*>(Interfaces::g_EntityList->GetClientEntity(index));
				if (!local)
					return;

				Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

				Ray_t ray;
				ray.Init(local->GetEyePosition(), position);

				CTraceFilter filter;
				filter.pSkip = local;

				trace_t tr;
				Interfaces::g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

				logs.push_back(cbullet_tracer_info(local->GetEyePosition(), position, Interfaces::g_Globals->curtime, g_Options.Colors.flTracers));


				if (!local)
					return;

				for (size_t i = 0; i < logs.size(); i++)
				{
					auto current = logs.at(i);
					current.color = Color(g_Options.Colors.flTracers); //color of local player's tracers
					BeamInfo_t beamInfo;
					beamInfo.m_nType = TE_BEAMPOINTS;
					beamInfo.m_pszModelName = "sprites/physbeam.vmt";
					beamInfo.m_nModelIndex = -1;
					beamInfo.m_flHaloScale = 0.0f;
					beamInfo.m_flLife = g_Options.Visuals.Bullettracersduration;
					beamInfo.m_flWidth = g_Options.Visuals.Bullettracerswidth;
					beamInfo.m_flEndWidth = g_Options.Visuals.Bullettracerswidth;
					beamInfo.m_flFadeLength = 0.0f;
					beamInfo.m_flAmplitude = 2.0f;
					beamInfo.m_flBrightness = 255.f;
					beamInfo.m_flSpeed = 0.2f;
					beamInfo.m_nStartFrame = 0;
					beamInfo.m_flFrameRate = 0.f;
					beamInfo.m_flRed = current.color.r();
					beamInfo.m_flGreen = current.color.g();
					beamInfo.m_flBlue = current.color.b();
					beamInfo.m_nSegments = 2;
					beamInfo.m_bRenderable = true;
					beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

					beamInfo.m_vecStart = LocalPlayer->GetEyePosition();
					beamInfo.m_vecEnd = current.dst;
					if (g_Options.Visuals.Bullettracers && g_Options.Visuals.Active)
					{
						auto beam = Interfaces::g_pViewRenderBeams->CreateBeamPoints(beamInfo);
						if (beam)
							Interfaces::g_pViewRenderBeams->DrawBeam(beam);
					}

					logs.erase(logs.begin() + i);
				}
			}

		}

	}



private:
	item_purchase_listener  listener;
};

item_purchase purchase;