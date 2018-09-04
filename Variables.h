#pragma once
#include <set>
#include <map>
#include <unordered_map>


struct skinInfo
{
	int seed = -1;
	int paintkit;
	std::string tagName;
};


//Color Feature for Color Changer
struct ColorP
{
public:
	const char* Name;
	float* Ccolor;

	ColorP(const char* name, float* color)
	{
		this->Name = name;
		this->Ccolor = color;
	}
};

struct Variables
{
	struct
	{
		bool Enable;
		bool AutoPistol;
		int MainKey = 1;
		
		float Mainfov;
		float MainSmooth;
		float main_random_smooth;
		float main_recoil_min;
		float main_recoil_max;
		float main_randomized_angle;

		float Pistolfov;
		float PistolSmooth;
		float pistol_random_smooth;
		float pistol_recoil_min;
		float pistol_recoil_max;
		float pistol_randomized_angle;

		float Sniperfov;
		float SniperSmooth;
		float sniper_random_smooth;
		float sniper_recoil_min;
		float sniper_recoil_max;
		float sniper_randomized_angle;

		float smg_fov;
		float smg_Smooth;
		float smg_random_smooth;
		float smg_recoil_min;
		float smg_recoil_max;
		float smg_randomized_angle;

		float heavy_wp_fov;
		float heavy_wp_Smooth;
		float heavy_wp_random_smooth;
		float heavy_wp_recoil_min;
		float heavy_wp_recoil_max;
		float heavy_wp_randomized_angle;

		struct
		{
			bool Headwpmain;
			bool Neckwpmain;
			bool Chestwpmain;
			bool Stomachwpmain;
			bool multiboneswpmain;
		}FilterMainWeapons;

		struct
		{
			bool Headwppis;
			bool Neckwppis;
			bool Chestwppis;
			bool Stomachwppis;
			bool multiboneswppis;
		}FilterPistolWeapons;


		struct
		{
			bool HeadwpSnip;
			bool NeckwpSnip;
			bool ChestwpSnip;
			bool StomachwpSnip;
			bool multiboneswpSnip;

		}FilterSniperWeapons;

		struct
		{
			bool Headwpsmg;
			bool Neckwpsmg;
			bool Chestwpsmg;
			bool Stomachwpsmg;
			bool multiboneswpsmg;

		}FiltersmgWeapons;

		struct
		{
			bool Headwphv;
			bool Neckwphv;
			bool Chestwphv;
			bool Stomachwphv;
			bool multibonewphv;

		}FilterheavyWeapons;

		struct
		{
			bool Enabled;
			float Delay;
			int Key = 1;
			float hitchance;
			struct
			{
				bool Head;
				bool Arms;
				bool Chest;
				bool Stomach;
				bool Legs;
			} Filter;

		} Triggerbot;

	}LegitBot;

	struct
	{
		bool backtrackenable;
		bool backtrackdots;
		int backtrackticks = 1;
	}Backtrack;

	struct
	{
		bool legitaaenable;
		int legitaatype;
	}LegitAA;

	struct
	{
		bool Active;

		//General ESP
		bool Box;
		int Boxtype;
		bool TeamESP;
		bool FillESP;
		float FillESPAmount;
		bool Name;
		bool Weapon;
		int WeaponType;
		bool Snapline;
		bool Health;
		bool Armor;
		int Armor2;
		bool Bomb;
		bool Skeleton;
		bool Skeletonhealth;

		//Others
		bool Damageindicator;
		bool Aimdirection;
		int Aimdirectionlength;
		bool Bullettracers;
		float Bullettracersduration = 1;
		float Bullettracerswidth = 1.5;

		//FOV
		bool FOV;
		float FOVChanger;
		bool VFOV;
		float VFOVChanger = 68;

		//Removals
		int Smoke;
		bool Noflash;
		bool Noscopeborder;
		bool Novisualrecoil;
		bool Nopostprocess;

		//Enemy State
		bool Flags;
		bool Callout;
		bool Distance;
		bool Money;

		//World
		int GrenadeType;
		bool Grenades;
		bool Grenadetrajectory;
		bool Droppedguns;
		int Droppedgunstype;
		bool Nightmode;
		bool Ambientlight;

		//Crosshair & Hitmarker
		bool Snipercrosshair;
		int Snipercrosshairtype;
		bool Recoilcrosshair;
		int Recoilcrosshairtype;
		bool Spreadcrosshair;
		float Spreadcrosshairalpha = 50;
		bool Hitmarker;
		int Hitsound;

		//Thirdperson
		bool Thirdperson;
		int Thirdpersonkey;
		bool Thirdpersongrenade;

		//Chams
		int Chamsstyle;
		bool Chamsplayer;
		bool Chamsteam;
		bool Chamshealth;
		bool Chamsxqz;

		//Hands & Weapons
		int Handstyle;
		int Weaponstyle;

		//Glow
		bool Glowplayer;
		float Glowplayeralpha = 255;
		bool Glowteam;
		float Glowteamalpha = 255;
		bool Glowweapon;
		float Glowweaponalpha = 255;
		bool Glowbomb;
		float Glowbombalpha = 255;

	}Visuals;

	struct
	{
		bool syncclantag;
		bool nameshitpaste;
		bool Bhop;
		int spammer;
		bool spammeron;
		int AutoStrafe;
		bool SpecList;
		bool ServerRankRevealAll;
		bool AutoAccept;
		bool Hitmarker;
		int Hitsound = 0;
		bool afkbot;
		bool inventoryalwayson;
		bool eventlogs2;
		bool radaringame;
		bool radarwindow;
		int radrsize = 200;
		float radralpha = 255;
		float radrzoom = 2.f;
		bool FakeLag;
		int FakeLagSlider;
	}Misc;

	struct
	{
		bool Enabled;
		int knifemodel;
		int glove;
		int gloveskin;

		struct
		{
			bool ChangerEnabled = false;
			int ChangerSkin = 0;
		} W[519]; // CHANGE HERE
		bool SkinFilter;
		std::unordered_map<std::string, std::set<std::string>> weaponSkins;
		std::unordered_map<std::string, skinInfo> skinMap;
		std::unordered_map<std::string, std::string> skinNames;
		std::vector<std::string> weaponNames;
	}Skinchanger;

	struct
	{
		bool Opened = false;
		int 	Key;
		int		ConfigFile = 0;
		int		Theme = 0;
		int currentWeapon;
	}Menu;

	struct
	{//need more
		float TeamESP[3] = { 0, 1.f, 0 };
		float EnemyESP[3] = { 1.f, 0, 0 };
		float EnemyChamsVis[3] = { 1.f, 0, 0 };
		float EnemyChamsNVis[3] = { 1.f, 0, 0 };
		float TeamChamsVis[3] = { 0, 1.f, 0 };
		float TeamChamsNVis[3] = { 0, 1.f, 0 };
		float PlayerChamsVis[3] = { 1.f, 1.f, 1.f };
		float PlayerChamsNVis[3] = { 0, 1.f, 1.f };
		float hitmarker_color[3] = { 1.f, 1.f, 1.f };
		float backtrackdots_color[3] = { 1.f, 1.f, 1.f };
		float dlight_color[3] = { 1.f, 1.f, 1.f };
		float color_skeleton[3] = { 1.f, 1.f, 1.f };
		float color_grenadeprediction[3] = { 1.f, 1.f, 1.f };
		float color_grenadeprediction_circle[3] = { 1.f, 1.f, 1.f };
		float GrenadeCollision[3] = { 1.f,0.f,0.f };
		float color_recoil[3] = { 1.f, 1.f, 1.f };
		float color_spread[3] = { 1.f, 1.f, 1.f };
		float color_sniper[3] = { 1.f, 1.f, 1.f };
		float EnemyGlow[3] = { 1.f, 1.f, 1.f };
		float TeamGlow[3] = { 1.f, 1.f, 1.f };
		float OtherGlow[3] = { 1.f, 1.f, 1.f };
		float HandsColor[3] = { 1.f, 1.f, 1.f };
		float AimLineColor[3] = { 1.f, 1.f, 1.f };
		float BulletTraceColor[3] = { 1.f, 1.f, 1.f };
		float fill_color_enemy[3] = { 1.f, 1.f, 0.0f };
		float fill_color_team[3] = { 0.f, 0.2f, 1.f };
		float glow_weapon[3] = { 1.f, 1.f, 1.f };
		float glow_c4[3] = { 1.f, 1.f, 1.f };
		float damageindicator[3] = { 1.f,1.f,1.f };
		float WeaponColor[3] = { 1.f, 1.f, 1.f };
		float droppedguns[3] = { 1.f,1.f,1.f };
		float ScopeLine[3] = { 0.f,0.f,0.f };
		float flTracers[3] = { 1.f, 0.f, 0.f };
		float ambientlightcolor[3];
		float hitbox[3] = { 1.f, 0.f, 0.f };
		float FakeAngleChams[3] = { 1.f, 1.f, 1.f };
	}Colors;
};

extern Variables g_Options;