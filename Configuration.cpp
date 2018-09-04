#include "Configuration.hpp"
#include "Variables.h"
#include <winerror.h>
#pragma warning( disable : 4091)
#include <ShlObj.h>
#include <string>
#include <sstream>
#include "XorStr.hpp"


const char* GetWeaponName(int iWeaponID)
{
	switch (iWeaponID)
	{
	default:
		return "none";
	case WEAPON_DEAGLE:
		return "deagle";
	case WEAPON_DUALS:
		return "duals";
	case WEAPON_FIVE7:
		return "five7";
	case WEAPON_GLOCK:
		return "glock";
	case WEAPON_AK47:
		return "ak47";
	case WEAPON_AUG:
		return "aug";
	//case WEAPON_MP5SD:
		//return "mp5";
	case WEAPON_AWP:
		return "awp";
	case WEAPON_FAMAS:
		return "famas";
	case WEAPON_G3SG1:
		return "g3sg1";
	case WEAPON_GALIL:
		return "galil";
	case WEAPON_M249:
		return "m249";
	case WEAPON_M4A1:
		return "m4a4";
	case WEAPON_MAC10:
		return "mac10";
	case WEAPON_P90:
		return "p90";
	case WEAPON_UMP45:
		return "ump45";
	case WEAPON_XM1014:
		return "xm1014";
	case WEAPON_BIZON:
		return "bizon";
	case WEAPON_MAG7:
		return "mag7";
	case WEAPON_NEGEV:
		return "negev";
	case WEAPON_SAWEDOFF:
		return "sawedoff";
	case WEAPON_TEC9:
		return "tec9";
	case WEAPON_TASER:
		return "taser";
	case WEAPON_USPS:
		return "usp-s";
	case WEAPON_MP7:
		return "mp7";
	case WEAPON_MP9:
		return "mp9";
	case WEAPON_NOVA:
		return "nova";
	case WEAPON_P250:
		return "p250";
	case WEAPON_SCAR20:
		return "scar20";
	case WEAPON_SG553:
		return "sg556";
	case WEAPON_SCOUT:
		return "ssg08";
	case WEAPON_M4A1S:
		return "m4a1-s";
	case WEAPON_P2000:
		return "p2000";
	case WEAPON_CZ75:
		return "cz75";
	case WEAPON_REVOLVER:
		return "revolver";
	case WEAPON_KNIFE_BAYONET:
		return "bayonet";
	case WEAPON_KNIFE_BUTTERFLY:
		return "butterfly";
	case WEAPON_KNIFE_FALCHION:
		return "falshion";
	case WEAPON_KNIFE_FLIP:
		return "flip";
	case WEAPON_KNIFE_GUT:
		return "gut";
	case WEAPON_KNIFE_KARAMBIT:
		return "karambit";
	case WEAPON_KNIFE_M9BAYONET:
		return "m9";
	case WEAPON_KNIFE_HUNTSMAN:
		return "huntsman";
	case WEAPON_KNIFE_BOWIE:
		return "bowie";
	case WEAPON_KNIFE_DAGGER:
		return "daggers";
	}
}

void CConfig::Setup()
{


	//BACKTRACK
	SetupValue(g_Options.Backtrack.backtrackenable, false, ("LegitBot"), ("Backtrack"));
	SetupValue(g_Options.Backtrack.backtrackdots, false, ("LegitBot"), ("backtrackdots"));
	SetupValue(g_Options.Backtrack.backtrackticks, 0, ("LegitBot"), ("BacktrackTicks"));
	
	SetupValue(g_Options.LegitAA.legitaaenable, false, ("LegitBot"), ("Legit-AA"));
	SetupValue(g_Options.LegitAA.legitaatype, 0, ("LegitBot"), ("LegitAAType"));

	//LEGITBOT
	SetupValue(g_Options.LegitBot.Enable, false, ("LegitBot"), ("Enabled"));
	SetupValue(g_Options.LegitBot.MainKey, 1, ("LegitBot"), ("Key-Aimbot"));

	SetupValue(g_Options.LegitBot.Mainfov, 0.f, ("LegitBot"), ("FOV-Rifles"));
	SetupValue(g_Options.LegitBot.MainSmooth, 0.f, ("LegitBot"), ("Smooth-Rifles"));
	SetupValue(g_Options.LegitBot.main_recoil_min, 0, ("LegitBot"), ("RCS-min-Rifles"));
	SetupValue(g_Options.LegitBot.main_recoil_max, 0, ("LegitBot"), ("RCS-max-Rifles"));

	SetupValue(g_Options.LegitBot.Pistolfov, 0.f, ("LegitBot"), ("FOV-Pistol"));
	SetupValue(g_Options.LegitBot.PistolSmooth, 0.f, ("LegitBot"), ("Smooth-Pistol"));
	SetupValue(g_Options.LegitBot.pistol_recoil_min, 0, ("LegitBot"), ("RCS-min-pistol"));
	SetupValue(g_Options.LegitBot.pistol_recoil_max, 0, ("LegitBot"), ("RCS-max-pistol"));

	SetupValue(g_Options.LegitBot.Sniperfov, 0.f, ("LegitBot"), ("FOV-Sniper"));
	SetupValue(g_Options.LegitBot.SniperSmooth, 0.f, ("LegitBot"), ("Smooth-Sniper"));
	SetupValue(g_Options.LegitBot.sniper_recoil_min, 0, ("LegitBot"), ("RCS-min-sniper"));
	SetupValue(g_Options.LegitBot.sniper_recoil_max, 0, ("LegitBot"), ("RCS-max-sniper"));

	SetupValue(g_Options.LegitBot.smg_fov, 0.f, ("LegitBot"), ("FOV-SMG"));
	SetupValue(g_Options.LegitBot.smg_Smooth, 0.f, ("LegitBot"), ("Smooth-SMG"));
	SetupValue(g_Options.LegitBot.smg_recoil_min, 0, ("LegitBot"), ("RCS-min-SMG"));
	SetupValue(g_Options.LegitBot.smg_recoil_max, 0, ("LegitBot"), ("RCS-max-SMG"));

	SetupValue(g_Options.LegitBot.heavy_wp_fov, 0.f, ("LegitBot"), ("FOV-heavy"));
	SetupValue(g_Options.LegitBot.heavy_wp_Smooth, 0.f, ("LegitBot"), ("Smooth-heavy"));
	SetupValue(g_Options.LegitBot.heavy_wp_recoil_min, 0, ("LegitBot"), ("RCS-min-heavy"));
	SetupValue(g_Options.LegitBot.heavy_wp_recoil_max, 0, ("LegitBot"), ("RCS-max-heavy"));

	//MAIN SETTS 
	SetupValue(g_Options.LegitBot.FilterMainWeapons.Headwpmain, false, ("LegitBot"), ("Headwpmain"));
	SetupValue(g_Options.LegitBot.FilterMainWeapons.Neckwpmain, false, ("LegitBot"), ("Neckwpmain"));
	SetupValue(g_Options.LegitBot.FilterMainWeapons.Chestwpmain, false, ("LegitBot"), ("Chestwpmain"));
	SetupValue(g_Options.LegitBot.FilterMainWeapons.Stomachwpmain, false, ("LegitBot"), ("Stomachwpmain"));
	SetupValue(g_Options.LegitBot.FilterMainWeapons.multiboneswpmain, false, ("LegitBot"), ("multiboneswpmain"));

	//PISTOLS
	SetupValue(g_Options.LegitBot.FilterPistolWeapons.Headwppis, false, ("LegitBot"), ("Headwppis"));
	SetupValue(g_Options.LegitBot.FilterPistolWeapons.Neckwppis, false, ("LegitBot"), ("Neckwppis"));
	SetupValue(g_Options.LegitBot.FilterPistolWeapons.Chestwppis, false, ("LegitBot"), ("Chestwppis"));
	SetupValue(g_Options.LegitBot.FilterPistolWeapons.Stomachwppis, false, ("LegitBot"), ("Stomachwppis"));
	SetupValue(g_Options.LegitBot.FilterPistolWeapons.multiboneswppis, false, ("LegitBot"), ("multiboneswppis"));

	//SNIPER
	SetupValue(g_Options.LegitBot.FilterSniperWeapons.HeadwpSnip, false, ("LegitBot"), ("HeadwpSnip"));
	SetupValue(g_Options.LegitBot.FilterSniperWeapons.NeckwpSnip, false, ("LegitBot"), ("NeckwpSnip"));
	SetupValue(g_Options.LegitBot.FilterSniperWeapons.ChestwpSnip, false, ("LegitBot"), ("ChestwpSnip"));
	SetupValue(g_Options.LegitBot.FilterSniperWeapons.StomachwpSnip, false, ("LegitBot"), ("StomachwpSnip"));
	SetupValue(g_Options.LegitBot.FilterSniperWeapons.multiboneswpSnip, false, ("LegitBot"), ("multiboneswpSnip"));

	//SMGWEAPONS
	SetupValue(g_Options.LegitBot.FiltersmgWeapons.Headwpsmg, false, ("LegitBot"), ("Headwpsmg"));
	SetupValue(g_Options.LegitBot.FiltersmgWeapons.Neckwpsmg, false, ("LegitBot"), ("Neckwpsmg"));
	SetupValue(g_Options.LegitBot.FiltersmgWeapons.Chestwpsmg, false, ("LegitBot"), ("Chestwpsmg"));
	SetupValue(g_Options.LegitBot.FiltersmgWeapons.Stomachwpsmg, false, ("LegitBot"), ("Stomachwpsmg"));
	SetupValue(g_Options.LegitBot.FiltersmgWeapons.multiboneswpsmg, false, ("LegitBot"), ("multiboneswpsmg"));

	//HEAVYWEAPONS
	SetupValue(g_Options.LegitBot.FilterheavyWeapons.Headwphv, false, ("LegitBot"), ("Headwphv"));
	SetupValue(g_Options.LegitBot.FilterheavyWeapons.Neckwphv, false, ("LegitBot"), ("Neckwphv"));
	SetupValue(g_Options.LegitBot.FilterheavyWeapons.Chestwphv, false, ("LegitBot"), ("Chestwphv"));
	SetupValue(g_Options.LegitBot.FilterheavyWeapons.Stomachwphv, false, ("LegitBot"), ("Stomachwphv"));
	SetupValue(g_Options.LegitBot.FilterheavyWeapons.multibonewphv, false, ("LegitBot"), ("multibonewphv"));



	//TRIGGERBOT
	SetupValue(g_Options.LegitBot.Triggerbot.Enabled, false, ("Triggerbot"), ("Enabled"));
	SetupValue(g_Options.LegitBot.Triggerbot.Key, 6, ("Triggerbot"), ("Key"));
	SetupValue(g_Options.LegitBot.Triggerbot.Delay, 0.f, ("Triggerbot"), ("Delay"));
	SetupValue(g_Options.LegitBot.Triggerbot.hitchance, 0.f, ("Triggerbot"), ("Hitchance"));

	SetupValue(g_Options.LegitBot.Triggerbot.Filter.Head, false, ("Triggerbot"), ("FilterHead"));
	SetupValue(g_Options.LegitBot.Triggerbot.Filter.Chest, false, ("Triggerbot"), ("FilterChest"));
	SetupValue(g_Options.LegitBot.Triggerbot.Filter.Stomach, false, ("Triggerbot"), ("FilterStomach"));
	SetupValue(g_Options.LegitBot.Triggerbot.Filter.Arms, false, ("Triggerbot"), ("FilterArms"));
	SetupValue(g_Options.LegitBot.Triggerbot.Filter.Legs, false, ("Triggerbot"), ("FilterLegs"));

	//VISUALS

	SetupValue(g_Options.Visuals.Active, false, ("Visuals"), ("Active"));

	//General
	SetupValue(g_Options.Visuals.Box, false, ("Visuals"), ("Box"));
	SetupValue(g_Options.Visuals.TeamESP, false, ("Visuals"), ("TeamESP"));
	SetupValue(g_Options.Visuals.Boxtype, 0, ("Visuals"), ("Boxtype"));
	SetupValue(g_Options.Visuals.FillESP, false, ("Visuals"), ("FillESP"));
	SetupValue(g_Options.Visuals.FillESPAmount, 255, ("Visuals"), ("FillESPAmount"));
	SetupValue(g_Options.Visuals.Name, false, ("Visuals"), ("Name"));
	SetupValue(g_Options.Visuals.Weapon, false, ("Visuals"), ("Weapon"));
	SetupValue(g_Options.Visuals.WeaponType, 0, ("Visuals"), ("WeaponType"));
	SetupValue(g_Options.Visuals.Health, false, ("Visuals"), ("Health"));
	SetupValue(g_Options.Visuals.Armor, false, ("Visuals"), ("Armor"));
	SetupValue(g_Options.Visuals.Armor2, 0, ("Visuals"), ("Armor2"));
	SetupValue(g_Options.Visuals.Bomb, false, ("Visuals"), ("Bomb"));
	SetupValue(g_Options.Visuals.Skeleton, false, ("Visuals"), ("Skeleton"));
	SetupValue(g_Options.Visuals.Skeletonhealth, false, ("Visuals"), ("Skeletonhealth"));

	//Others
	SetupValue(g_Options.Visuals.Damageindicator, false, ("Visuals"), ("Damageindicator"));
	SetupValue(g_Options.Visuals.Aimdirection, false, ("Visuals"), ("Aimdirection"));
	SetupValue(g_Options.Visuals.Aimdirectionlength, 0, ("Visuals"), ("Aimdirectionlength"));
	SetupValue(g_Options.Visuals.Bullettracers, false, ("Visuals"), ("Bullettracers"));
	SetupValue(g_Options.Visuals.Bullettracersduration, 0.f, ("Visuals"), ("Bullettracersduration"));
	SetupValue(g_Options.Visuals.Bullettracerswidth, 0.f, ("Visuals"), ("Bullettracerswidth"));

	//FOV
	SetupValue(g_Options.Visuals.FOV, false, ("Visuals"), ("FOV"));
	SetupValue(g_Options.Visuals.FOVChanger, 0.f, ("Visuals"), ("FOVChanger"));
	SetupValue(g_Options.Visuals.VFOV, false, ("Visuals"), ("VFOV"));
	SetupValue(g_Options.Visuals.VFOVChanger, 0.f, ("Visuals"), ("VFOVChanger"));

	//Removals
	SetupValue(g_Options.Visuals.Smoke, 0, ("Visuals"), ("Smoke"));
	SetupValue(g_Options.Visuals.Noflash, false, ("Visuals"), ("Noflash"));
	SetupValue(g_Options.Visuals.Noscopeborder, false, ("Visuals"), ("Noscopeborder"));
	SetupValue(g_Options.Visuals.Novisualrecoil, false, ("Visuals"), ("Novisualrecoil"));
	SetupValue(g_Options.Visuals.Nopostprocess, false, ("Visuals"), ("Nopostprocess"));

	//Enemy State
	SetupValue(g_Options.Visuals.Flags, false, ("Visuals"), ("Flags"));
	SetupValue(g_Options.Visuals.Callout, false, ("Visuals"), ("Callout"));
	SetupValue(g_Options.Visuals.Distance, false, ("Visuals"), ("Distance"));
	SetupValue(g_Options.Visuals.Money, false, ("Visuals"), ("Money"));

	//World
	SetupValue(g_Options.Visuals.Grenades, false, ("Visuals"), ("Grenades"));
	SetupValue(g_Options.Visuals.GrenadeType, 0, ("Visuals"), ("GrenadeType"));
	SetupValue(g_Options.Visuals.Grenadetrajectory, false, ("Visuals"), ("Grenadetrajectory"));
	SetupValue(g_Options.Visuals.Droppedguns, false, ("Visuals"), ("Droppedguns"));
	SetupValue(g_Options.Visuals.Droppedgunstype, 0, ("Visuals"), ("Droppedgunstype"));
	SetupValue(g_Options.Visuals.Nightmode, false, ("Visuals"), ("Nightmode"));
	SetupValue(g_Options.Visuals.Ambientlight, false, ("Visuals"), ("Ambientlight"));

	//Crosshair & Hitmarker
	SetupValue(g_Options.Visuals.Snipercrosshair, false, ("Visuals"), ("Snipercrosshair"));
	SetupValue(g_Options.Visuals.Snipercrosshairtype, 0, ("Visuals"), ("Snipercrosshairtype"));
	SetupValue(g_Options.Visuals.Recoilcrosshair, false, ("Visuals"), ("Recoilcrosshair"));
	SetupValue(g_Options.Visuals.Recoilcrosshairtype, 0, ("Visuals"), ("Recoilcrosshairtype"));
	SetupValue(g_Options.Visuals.Spreadcrosshair, false, ("Visuals"), ("Spreadcrosshair"));
	SetupValue(g_Options.Visuals.Spreadcrosshairalpha, 0.f, ("Visuals"), ("Spreadcrosshairalpha"));
	SetupValue(g_Options.Visuals.Hitmarker, false, ("Visuals"), ("Hitmarker"));
	SetupValue(g_Options.Visuals.Hitsound, 0, ("Visuals"), ("Hitsound"));

    //Thirdperson
	SetupValue(g_Options.Visuals.Thirdperson, false, ("Visuals"), ("Thirdperson"));
	SetupValue(g_Options.Visuals.Thirdpersonkey, 0, ("Visuals"), ("Thirdpersonkey"));
	SetupValue(g_Options.Visuals.Thirdpersongrenade, false, ("Visuals"), ("Thirdpersongrenade"));

	//Player Chams
	SetupValue(g_Options.Visuals.Chamsstyle, 0, ("Visuals"), ("Chamsstyle"));
	SetupValue(g_Options.Visuals.Chamsplayer, false, ("Visuals"), ("Chamsplayer"));
	SetupValue(g_Options.Visuals.Chamshealth, false, ("Visuals"), ("Chamshealth"));
	SetupValue(g_Options.Visuals.Chamsteam, false, ("Visuals"), ("Chamsteam"));
	SetupValue(g_Options.Visuals.Chamsxqz, false, ("Visuals"), ("Chamsxqz"));

	//Hands & Weapons
	SetupValue(g_Options.Visuals.Handstyle, 0, ("Visuals"), ("Handstyle"));
	SetupValue(g_Options.Visuals.Weaponstyle, 0, ("Visuals"), ("Weaponstyle"));

	//Glow
	SetupValue(g_Options.Visuals.Glowplayer, false, ("Visuals"), ("Glowplayer"));
	SetupValue(g_Options.Visuals.Glowplayeralpha, 0.f, ("Visuals"), ("Glowplayeralpha"));
	SetupValue(g_Options.Visuals.Glowteam, false, ("Visuals"), ("Glowteam"));
	SetupValue(g_Options.Visuals.Glowteamalpha, 0.f, ("Visuals"), ("Glowteamalpha"));
	SetupValue(g_Options.Visuals.Glowweapon, false, ("Visuals"), ("Glowweapon"));
	SetupValue(g_Options.Visuals.Glowweaponalpha, 0.f, ("Visuals"), ("Glowweaponalpha"));
	SetupValue(g_Options.Visuals.Glowbomb, false, ("Visuals"), ("Glowbomb"));
	SetupValue(g_Options.Visuals.Glowbombalpha, 0.f, ("Visuals"), ("Glowbombalpha"));


	SetupValue(g_Options.Misc.syncclantag, false, ("Misc"), ("Sync-Clantag"));
	SetupValue(g_Options.Misc.nameshitpaste, false, ("Misc"), ("nameshitpaste"));
	SetupValue(g_Options.Misc.Bhop, false, ("Misc"), ("Bhop"));
	SetupValue(g_Options.Misc.AutoStrafe, 0, ("Misc"), ("Autostrafe"));
	SetupValue(g_Options.Misc.spammer, 0, ("Visuals"), ("Spammer"));
	SetupValue(g_Options.Misc.spammeron, false, ("Misc"), ("SpammerEnabled"));
	SetupValue(g_Options.Misc.SpecList, false, ("Misc"), ("SpecList"));
	SetupValue(g_Options.Misc.ServerRankRevealAll, false, ("Misc"), ("RankRevealAll"));
	SetupValue(g_Options.Misc.AutoAccept, false, ("Misc"), ("AutoAccept"));
	SetupValue(g_Options.Misc.Hitmarker, false, ("Misc"), ("Hitmarker"));
	SetupValue(g_Options.Misc.Hitsound, 0, ("Misc"), ("HitSound"));
	SetupValue(g_Options.Misc.FakeLag, false, ("Misc"), ("FakeLag"));
	SetupValue(g_Options.Misc.FakeLagSlider, 0, ("Misc"), ("FakeLagSlider"));
	SetupValue(g_Options.Misc.eventlogs2, false, ("Misc"), ("Log2"));
	SetupValue(g_Options.Misc.afkbot, false, ("Misc"), ("AFK-BOT"));

	SetupValue(g_Options.Misc.inventoryalwayson, false, ("Misc"), ("EnableInventory"));

	SetupValue(g_Options.Misc.radarwindow, false, ("Misc"), ("Radar-Window"));

	SetupValue(g_Options.Misc.radrzoom, 1.50, ("Misc"), ("Radar-Zoom"));
	SetupValue(g_Options.Misc.radralpha, 255, ("Misc"), ("Radar-Alpha"));
	SetupValue(g_Options.Misc.radrsize, 200, ("Misc"), ("Radar-Size"));

	SetupValue(g_Options.Skinchanger.Enabled, true, ("skins"), ("enable"));
	SetupValue(g_Options.Skinchanger.gloveskin, 0, ("skins"), ("gloveskin"));
	SetupValue(g_Options.Skinchanger.glove, 0, ("skins"), ("glove"));
	SetupValue(g_Options.Skinchanger.knifemodel, 0, ("skins"), ("modelknife"));

	for (int id = 0; id < 65; id++) 
	{ // Weapons
		SetupValue(g_Options.Skinchanger.W[id].ChangerSkin, 0, GetWeaponName(id), ("Skin"));
	}

	for (int id = 500; id <= 516; id++) 
	{ // knifes
		if ((id < 505 && id >= 501) || (id > 512 && id < 514))
			continue;

		SetupValue(g_Options.Skinchanger.W[id].ChangerSkin, 0, GetWeaponName(id), ("Skin"));
	}


	//COLORS

	SetupValue(g_Options.Colors.TeamESP[0], 0.0f, "Colors", "TeamESP1");
	SetupValue(g_Options.Colors.TeamESP[1], 0.0f, "Colors", "TeamESP2");
	SetupValue(g_Options.Colors.TeamESP[2], 0.0f, "Colors", "TeamESP3");

	SetupValue(g_Options.Colors.EnemyESP[0], 0.0f, "Colors", "EnemyESP1");
	SetupValue(g_Options.Colors.EnemyESP[1], 0.0f, "Colors", "EnemyESP2");
	SetupValue(g_Options.Colors.EnemyESP[2], 0.0f, "Colors", "EnemyESP3");

	SetupValue(g_Options.Colors.EnemyChamsVis[0], 0.0f, "Colors", "EnemyChamsVis1");
	SetupValue(g_Options.Colors.EnemyChamsVis[1], 0.0f, "Colors", "EnemyChamsVis2");
	SetupValue(g_Options.Colors.EnemyChamsVis[2], 0.0f, "Colors", "EnemyChamsVis3");

	SetupValue(g_Options.Colors.EnemyChamsNVis[0], 0.0f, "Colors", "EnemyChamsNVis1");
	SetupValue(g_Options.Colors.EnemyChamsNVis[1], 0.0f, "Colors", "EnemyChamsNVis2");
	SetupValue(g_Options.Colors.EnemyChamsNVis[2], 0.0f, "Colors", "EnemyChamsNVis3");

	SetupValue(g_Options.Colors.TeamChamsVis[0], 0.0f, "Colors", "TeamChamsVis1");
	SetupValue(g_Options.Colors.TeamChamsVis[1], 0.0f, "Colors", "TeamChamsVis2");
	SetupValue(g_Options.Colors.TeamChamsVis[2], 0.0f, "Colors", "TeamChamsVis3");

	SetupValue(g_Options.Colors.TeamChamsNVis[0], 0.0f, "Colors", "TeamChamsNVi1");
	SetupValue(g_Options.Colors.TeamChamsNVis[1], 0.0f, "Colors", "TeamChamsNVi2");
	SetupValue(g_Options.Colors.TeamChamsNVis[2], 0.0f, "Colors", "TeamChamsNVi3");

	SetupValue(g_Options.Colors.hitmarker_color[0], 0.0f, "Colors", "hitmarker_color1");
	SetupValue(g_Options.Colors.hitmarker_color[1], 0.0f, "Colors", "hitmarker_color2");
	SetupValue(g_Options.Colors.hitmarker_color[2], 0.0f, "Colors", "hitmarker_color3");

	SetupValue(g_Options.Colors.backtrackdots_color[0], 0.0f, "Colors", "backtrackdots_color1");
	SetupValue(g_Options.Colors.backtrackdots_color[1], 0.0f, "Colors", "backtrackdots_color2");
	SetupValue(g_Options.Colors.backtrackdots_color[2], 0.0f, "Colors", "backtrackdots_color3");

	SetupValue(g_Options.Colors.hitbox[0], 0.0f, "Colors", "hitbox1");
	SetupValue(g_Options.Colors.hitbox[1], 0.0f, "Colors", "hitbox2");
	SetupValue(g_Options.Colors.hitbox[2], 0.0f, "Colors", "hitbox3");

	SetupValue(g_Options.Colors.dlight_color[0], 0.0f, "Colors", "dlight_color1");
	SetupValue(g_Options.Colors.dlight_color[1], 0.0f, "Colors", "dlight_color2");
	SetupValue(g_Options.Colors.dlight_color[2], 0.0f, "Colors", "dlight_color3");

	SetupValue(g_Options.Colors.color_skeleton[0], 0.0f, "Colors", "color_skeleton1");
	SetupValue(g_Options.Colors.color_skeleton[1], 0.0f, "Colors", "color_skeleton2");
	SetupValue(g_Options.Colors.color_skeleton[2], 0.0f, "Colors", "color_skeleton3");

	SetupValue(g_Options.Colors.color_grenadeprediction[0], 0.0f, "Colors", "color_grenadeprediction1");
	SetupValue(g_Options.Colors.color_grenadeprediction[1], 0.0f, "Colors", "color_grenadeprediction2");
	SetupValue(g_Options.Colors.color_grenadeprediction[2], 0.0f, "Colors", "color_grenadeprediction3");

	SetupValue(g_Options.Colors.color_grenadeprediction_circle[0], 0.0f, "Colors", "color_grenadeprediction_circle1");
	SetupValue(g_Options.Colors.color_grenadeprediction_circle[1], 0.0f, "Colors", "color_grenadeprediction_circle2");
	SetupValue(g_Options.Colors.color_grenadeprediction_circle[2], 0.0f, "Colors", "color_grenadeprediction_circle3");

	SetupValue(g_Options.Colors.GrenadeCollision[0], 0.0f, "Colors", "color_grenadeprediction_box1");
	SetupValue(g_Options.Colors.GrenadeCollision[1], 0.0f, "Colors", "color_grenadeprediction_box2");
	SetupValue(g_Options.Colors.GrenadeCollision[2], 0.0f, "Colors", "color_grenadeprediction_box3");

	SetupValue(g_Options.Colors.color_recoil[0], 0.0f, "Colors", "color_recoil1");
	SetupValue(g_Options.Colors.color_recoil[1], 0.0f, "Colors", "color_recoil2");
	SetupValue(g_Options.Colors.color_recoil[2], 0.0f, "Colors", "color_recoil3");

	SetupValue(g_Options.Colors.color_spread[0], 0.0f, "Colors", "color_spread1");
	SetupValue(g_Options.Colors.color_spread[1], 0.0f, "Colors", "color_spread2");
	SetupValue(g_Options.Colors.color_spread[2], 0.0f, "Colors", "color_spread3");

	SetupValue(g_Options.Colors.color_sniper[0], 0.0f, "Colors", "color_sniper1");
	SetupValue(g_Options.Colors.color_sniper[1], 0.0f, "Colors", "color_sniper2");
	SetupValue(g_Options.Colors.color_sniper[2], 0.0f, "Colors", "color_sniper3");

	SetupValue(g_Options.Colors.EnemyGlow[0], 0.0f, "Colors", "EnemyGlow1");
	SetupValue(g_Options.Colors.EnemyGlow[1], 0.0f, "Colors", "EnemyGlow2");
	SetupValue(g_Options.Colors.EnemyGlow[2], 0.0f, "Colors", "EnemyGlow3");

	SetupValue(g_Options.Colors.FakeAngleChams[0], 0.0f, "Colors", "FakeAngleChams1");
	SetupValue(g_Options.Colors.FakeAngleChams[1], 0.0f, "Colors", "FakeAngleChams2");
	SetupValue(g_Options.Colors.FakeAngleChams[2], 0.0f, "Colors", "FakeAngleChams3");

	SetupValue(g_Options.Colors.TeamGlow[0], 0.0f, "Colors", "TeamGlow1");
	SetupValue(g_Options.Colors.TeamGlow[1], 0.0f, "Colors", "TeamGlow2");
	SetupValue(g_Options.Colors.TeamGlow[2], 0.0f, "Colors", "TeamGlow3");

	SetupValue(g_Options.Colors.HandsColor[0], 0.0f, "Colors", "HandsColor1");
	SetupValue(g_Options.Colors.HandsColor[1], 0.0f, "Colors", "HandsColor2");
	SetupValue(g_Options.Colors.HandsColor[2], 0.0f, "Colors", "HandsColor3");

	SetupValue(g_Options.Colors.WeaponColor[0], 0.0f, "Colors", "WeaponColor1");
	SetupValue(g_Options.Colors.WeaponColor[1], 0.0f, "Colors", "WeaponColor2");
	SetupValue(g_Options.Colors.WeaponColor[2], 0.0f, "Colors", "WeaponColor3");

	SetupValue(g_Options.Colors.AimLineColor[0], 0.0f, "Colors", "AimLineColor1");
	SetupValue(g_Options.Colors.AimLineColor[1], 0.0f, "Colors", "AimLineColor2");
	SetupValue(g_Options.Colors.AimLineColor[2], 0.0f, "Colors", "AimLineColor3");

	SetupValue(g_Options.Colors.ScopeLine[0], 0.0f, "Colors", "ScopeLineColor1");
	SetupValue(g_Options.Colors.ScopeLine[1], 0.0f, "Colors", "ScopeLineColor2");
	SetupValue(g_Options.Colors.ScopeLine[2], 0.0f, "Colors", "ScopeLineColor3");

	SetupValue(g_Options.Colors.BulletTraceColor[0], 0.0f, "Colors", "BulletTraceColor1");
	SetupValue(g_Options.Colors.BulletTraceColor[1], 0.0f, "Colors", "BulletTraceColor2");
	SetupValue(g_Options.Colors.BulletTraceColor[2], 0.0f, "Colors", "BulletTraceColor3");

	SetupValue(g_Options.Colors.fill_color_enemy[0], 0.0f, "Colors", "fill_color_enemy1");
	SetupValue(g_Options.Colors.fill_color_enemy[1], 0.0f, "Colors", "fill_color_enemy2");
	SetupValue(g_Options.Colors.fill_color_enemy[2], 0.0f, "Colors", "fill_color_enemy3");

	SetupValue(g_Options.Colors.fill_color_team[0], 0.0f, "Colors", "fill_color_team1");
	SetupValue(g_Options.Colors.fill_color_team[1], 0.0f, "Colors", "fill_color_team2");
	SetupValue(g_Options.Colors.fill_color_team[2], 0.0f, "Colors", "fill_color_team3");

	SetupValue(g_Options.Colors.glow_weapon[0], 0.0f, "Colors", "glow_weapon1");
	SetupValue(g_Options.Colors.glow_weapon[1], 0.0f, "Colors", "glow_weapon2");
	SetupValue(g_Options.Colors.glow_weapon[2], 0.0f, "Colors", "glow_weapon3");

	SetupValue(g_Options.Colors.glow_c4[0], 0.0f, "Colors", "glow_c41");
	SetupValue(g_Options.Colors.glow_c4[1], 0.0f, "Colors", "glow_c42");
	SetupValue(g_Options.Colors.glow_c4[2], 0.0f, "Colors", "glow_c43");

	SetupValue(g_Options.Colors.damageindicator[0], 0.0f, "Colors", "damageindicator1");
	SetupValue(g_Options.Colors.damageindicator[1], 0.0f, "Colors", "damageindicator2");
	SetupValue(g_Options.Colors.damageindicator[2], 0.0f, "Colors", "damageindicator3");

	SetupValue(g_Options.Colors.droppedguns[0], 0.0f, "Colors", "droppedguns1");
	SetupValue(g_Options.Colors.droppedguns[1], 0.0f, "Colors", "droppedguns2");
	SetupValue(g_Options.Colors.droppedguns[2], 0.0f, "Colors", "droppedguns3");

	SetupValue(g_Options.Colors.flTracers[0], 0.0f, "Colors", "flTracers_1");
	SetupValue(g_Options.Colors.flTracers[1], 0.0f, "Colors", "flTracers_2");
	SetupValue(g_Options.Colors.flTracers[2], 0.0f, "Colors", "flTracers_3");

	SetupValue(g_Options.Colors.ambientlightcolor[0], 0.0f, "Colors", "ambientlightcolor_1");
	SetupValue(g_Options.Colors.ambientlightcolor[1], 0.0f, "Colors", "ambientlightcolor_2");
	SetupValue(g_Options.Colors.ambientlightcolor[2], 0.0f, "Colors", "ambientlightcolor_3");
}

void CConfig::SetupValue(int &value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue<int>(category, name, &value));
}

void CConfig::SetupValue(float &value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue<float>(category, name, &value));
}

void CConfig::SetupValue(bool &value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue<bool>(category, name, &value));
}

void CConfig::Save(char* name)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;
	if (strlen(name) > 0)
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
		{
			folder = std::string(path) + ("\\dc\\");
			file = std::string(path) + ("\\dc\\") + std::string(name) + XorStr(".dc");
		}
	}
	else
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
		{
			folder = std::string(path) + ("\\dc\\");
			file = std::string(path) + ("\\dc\\config.dc");
		}
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load(char* name)
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;
	if (strlen(name) > 0)
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
		{
			folder = std::string(path) + ("\\dc\\");
			file = std::string(path) + ("\\dc\\") + std::string(name) + XorStr(".dc");
		}
	}
	else
	{
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
		{
			folder = std::string(path) + ("\\dc\\");
			file = std::string(path) + ("\\dc\\config.dc");
		}
	}

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = (float)atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}

CConfig* Config = new CConfig();
Variables g_Options;














































































































































































































































































































































































































































































































































































