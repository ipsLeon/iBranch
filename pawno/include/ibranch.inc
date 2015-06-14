/*
** ipsTeam Update Branches
** Created by ipsLeon
** Thanks to Y_Less for ALS Hook method 7
*/

forward OnPlayerSkinChange(playerid, newskin, oldskin);
forward OnPlayerNameChange(playerid, newname[], oldname[]);
forward OnPlayerMoneyChange(playerid, newmoney, oldmoney);
forward OnPlayerWantedLevelChange(playerid, newlevel, oldlevel);
forward OnPlayerScoreChange(playerid, newscore, oldscore);
forward OnPlayerWeaponSlotChange(playerid, slotid, new_weapon, oldweapon);
forward OnPlayerAmmoChange(playerid, weaponid, newammo, oldammo);
forward OnPlayerCurrentWeaponChange(playerid, new_weapon, oldweapon);
forward OnPlayerVehicleChange(playerid, new_vehicle, oldvehicle);
forward OnPlayerVehicleSeatChange(playerid, newseat, oldseat);
forward OnPlayerHealthChange(playerid, Float:newhealth, Float:oldhealth);
forward OnPlayerArmourChange(playerid, Float:newarmour, Float:oldarmour);
forward OnPlayerVirtualWorldChange(playerid, new_world, oldworld);
forward OnPlayerPingChange(playerid, newping, oldping);
forward OnPlayerFPSChange(playerid, newfps, oldfps);
forward OnPlayerDrunkLevelChange(playerid, newlevel, oldlevel);
forward OnPlayerFreezeStateChange(playerid, newstate, oldstate);
forward OnPlayerTeamChange(playerid, newteam, oldteam);
forward OnPlayerGenderChange(playerid, newgender, oldgender);

#if !defined strcpy
	#define strcpy(%0,%1) strcat((%0[0] = '\0', %0), %1)
#endif

#if !defined isnull
    #define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

#define SKIN_GENDER_NONE 0
#define SKIN_GENDER_FEMALE 1
#define SKIN_GENDER_MALE 2


enum UpdateData
{
	iB_Skin,
	iB_Gender,
	iB_Name[MAX_PLAYER_NAME],
	iB_Money,
	iB_WantedLevel,
	iB_Score,
	iB_Weapon[13],
	iB_Ammo[13],
	iB_CurrentWeapon,
	iB_Vehicle,
	iB_Seat,
	Float:iB_Health,
	Float:iB_Armour,
	iB_World,
	iB_Ping,
	iB_FPS,
	iB_DrunkLevel,
	bool:iB_Freezed,
	iB_Team,
	iB_UpdateTimer
}
new PlayerUpdateInfo[MAX_PLAYERS][UpdateData];

enum FPSData
{
	iB_PlayerFPS,
	iB_LastPlayerDrunkLevel,
	iB_NewPlayerDrunkLevel
}
new PlayerFPSInfo[MAX_PLAYERS][FPSData];

CleanPlayerUpdateInfo(playerid)
{	
	KillTimer(PlayerUpdateInfo[playerid][iB_UpdateTimer]);
	PlayerUpdateInfo[playerid][iB_Skin] = 0;
	PlayerUpdateInfo[playerid][iB_Gender] = SKIN_GENDER_NONE;
	PlayerUpdateInfo[playerid][iB_Name] = EOS;
	PlayerUpdateInfo[playerid][iB_Money] = 0;
	PlayerUpdateInfo[playerid][iB_WantedLevel] = 0;
	PlayerUpdateInfo[playerid][iB_Score] = 0;
	PlayerUpdateInfo[playerid][iB_Weapon] = EOS;
	PlayerUpdateInfo[playerid][iB_Ammo] = EOS;
	PlayerUpdateInfo[playerid][iB_CurrentWeapon] = 0;
	PlayerUpdateInfo[playerid][iB_Vehicle] = 0;
	PlayerUpdateInfo[playerid][iB_Seat] = -1;
	PlayerUpdateInfo[playerid][iB_Health] = 100.0;
	PlayerUpdateInfo[playerid][iB_Armour] = 0.0;
	PlayerUpdateInfo[playerid][iB_World] = 0;
	PlayerUpdateInfo[playerid][iB_Ping] = 0;
	PlayerUpdateInfo[playerid][iB_FPS] = 0;
	PlayerUpdateInfo[playerid][iB_DrunkLevel] = 0;
	PlayerUpdateInfo[playerid][iB_Freezed] = false;
	PlayerUpdateInfo[playerid][iB_Team] = 255;
	PlayerFPSInfo[playerid][iB_PlayerFPS] = 0;
	PlayerFPSInfo[playerid][iB_LastPlayerDrunkLevel] = 0;
	PlayerFPSInfo[playerid][iB_NewPlayerDrunkLevel] = 0;
	return 1;
}

public OnPlayerConnect(playerid)
{
    #if defined iBranch_OnPlayerConnect
        iBranch_OnPlayerConnect(playerid);
    #endif
	CleanPlayerUpdateInfo(playerid);
	PlayerUpdateInfo[playerid][iB_UpdateTimer] = SetTimerEx("iBranch_OnPlayerUpdate", 100, true, "i", playerid);
    return 1;
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect iBranch_OnPlayerConnect

#if defined iBranch_OnPlayerConnect
    forward iBranch_OnPlayerConnect(playerid);
#endif


forward iBranch_OnPlayerUpdate(playerid);
public iBranch_OnPlayerUpdate(playerid)
{
	if(!IsPlayerConnected(playerid)) return CleanPlayerUpdateInfo(playerid);
	new upd_skin = GetPlayerSkin(playerid),
	upd_name[MAX_PLAYER_NAME],
	upd_money = GetPlayerMoney(playerid),
	upd_wantedlevel = GetPlayerWantedLevel(playerid),
	upd_score = GetPlayerScore(playerid),
	upd_weapon[13],
	upd_ammo[13],
	upd_currentweapon = GetPlayerWeapon(playerid),
	upd_vehicle = GetPlayerVehicleID(playerid),
	upd_seat = GetPlayerVehicleSeat(playerid),
	Float:upd_health,
	Float:upd_armour,
	upd_world = GetPlayerVirtualWorld(playerid),
	upd_ping = GetPlayerPing(playerid),
	upd_fps = GetPlayerFPS(playerid),
	upd_drunklevel = GetPlayerDrunkLevel(playerid),
	upd_team = GetPlayerTeam(playerid);
	for(new i; i < 13; i++)	GetPlayerWeaponData(playerid, i, upd_weapon[i], upd_ammo[i]);
	GetPlayerHealth(playerid, upd_health);
	GetPlayerArmour(playerid, upd_armour);
	GetPlayerName(playerid, upd_name, sizeof(upd_name));	
	
	if(upd_skin != PlayerUpdateInfo[playerid][iB_Skin])
	{
		if(iB_GetSkinGender(upd_skin) != PlayerUpdateInfo[playerid][iB_Gender])
		{
			CallRemoteFunction("OnPlayerGenderChange", "iii", playerid, iB_GetSkinGender(upd_skin), iB_GetSkinGender(PlayerUpdateInfo[playerid][iB_Skin]));
			PlayerUpdateInfo[playerid][iB_Gender] = iB_GetSkinGender(upd_skin);
		}
		CallRemoteFunction("OnPlayerSkinChange", "iii", playerid, upd_skin, PlayerUpdateInfo[playerid][iB_Skin]);
		PlayerUpdateInfo[playerid][iB_Skin] = upd_skin;
	}
	if(strcmp(upd_name, PlayerUpdateInfo[playerid][iB_Name]))
	{
		if(!isnull(upd_name) && !isnull(PlayerUpdateInfo[playerid][iB_Name])) CallRemoteFunction("OnPlayerNameChange", "iss", playerid, upd_name, PlayerUpdateInfo[playerid][iB_Name]);
		strcpy(PlayerUpdateInfo[playerid][iB_Name], upd_name);
	}
	if(upd_money != PlayerUpdateInfo[playerid][iB_Money])
	{
		CallRemoteFunction("OnPlayerMoneyChange", "iiii", playerid, upd_money, PlayerUpdateInfo[playerid][iB_Money]);
		PlayerUpdateInfo[playerid][iB_Money] = upd_money;
	}
	if(upd_wantedlevel != PlayerUpdateInfo[playerid][iB_WantedLevel])
	{
		CallRemoteFunction("OnPlayerWantedLevelChange", "iii", playerid, upd_wantedlevel, PlayerUpdateInfo[playerid][iB_WantedLevel]);
		PlayerUpdateInfo[playerid][iB_WantedLevel] = upd_wantedlevel;
	}
	if(upd_score != PlayerUpdateInfo[playerid][iB_Score])
	{
		CallRemoteFunction("OnPlayerScoreChange", "idd", playerid, upd_score, PlayerUpdateInfo[playerid][iB_Score]);
		PlayerUpdateInfo[playerid][iB_Score] = upd_score;
	}
	for(new s; s < 13; s++)
	{
		if(upd_weapon[s] != PlayerUpdateInfo[playerid][iB_Weapon][s])
		{
			CallRemoteFunction("OnPlayerWeaponSlotChange", "iiii", playerid, s, upd_weapon[s], PlayerUpdateInfo[playerid][iB_Weapon][s]);
			PlayerUpdateInfo[playerid][iB_Weapon][s] = upd_weapon[s];
		}
		if(upd_ammo[s] != PlayerUpdateInfo[playerid][iB_Ammo][s])
		{
			CallRemoteFunction("OnPlayerAmmoChange", "iiii", playerid, upd_weapon[s], upd_ammo[s], PlayerUpdateInfo[playerid][iB_Ammo][s]);
			PlayerUpdateInfo[playerid][iB_Ammo][s] = upd_ammo[s];
		}
	}
	if(upd_currentweapon != PlayerUpdateInfo[playerid][iB_CurrentWeapon])
	{
		CallRemoteFunction("OnPlayerCurrentWeaponChange", "iii", playerid, upd_currentweapon, PlayerUpdateInfo[playerid][iB_CurrentWeapon]);
		PlayerUpdateInfo[playerid][iB_CurrentWeapon] = upd_currentweapon;
	}
	if(upd_vehicle != PlayerUpdateInfo[playerid][iB_Vehicle])
	{
		if(upd_vehicle != 128)
		{
			CallRemoteFunction("OnPlayerVehicleChange", "iii", playerid, upd_vehicle, PlayerUpdateInfo[playerid][iB_Vehicle]);
			PlayerUpdateInfo[playerid][iB_Vehicle] =  upd_vehicle;
		}
	}
	if(upd_seat != PlayerUpdateInfo[playerid][iB_Seat])
	{
		CallRemoteFunction("OnPlayerVehicleSeatChange", "iii", playerid, upd_seat, PlayerUpdateInfo[playerid][iB_Seat]);
		PlayerUpdateInfo[playerid][iB_Seat] = upd_seat;
	}
	if(upd_health != PlayerUpdateInfo[playerid][iB_Health])
	{
		CallRemoteFunction("OnPlayerHealthChange", "iff", playerid, upd_health, PlayerUpdateInfo[playerid][iB_Health]);
		PlayerUpdateInfo[playerid][iB_Health] = upd_health;
	}
	if(upd_armour != PlayerUpdateInfo[playerid][iB_Armour])
	{
		CallRemoteFunction("OnPlayerArmourChange", "iff", playerid, upd_armour, PlayerUpdateInfo[playerid][iB_Armour]);
		PlayerUpdateInfo[playerid][iB_Armour] = upd_armour;
	}
	if(upd_world != PlayerUpdateInfo[playerid][iB_World])
	{
		CallRemoteFunction("OnPlayerVirtualWorldChange", "iii", playerid, upd_world, PlayerUpdateInfo[playerid][iB_World]);
		PlayerUpdateInfo[playerid][iB_World] = upd_world;
	}
	if(upd_ping != PlayerUpdateInfo[playerid][iB_Ping])
	{
		CallRemoteFunction("OnPlayerPingChange", "iii", playerid, upd_ping, PlayerUpdateInfo[playerid][iB_Ping]);
		PlayerUpdateInfo[playerid][iB_Ping] = upd_ping;
	}
	if(upd_fps != PlayerUpdateInfo[playerid][iB_FPS])
	{
		CallRemoteFunction("OnPlayerFPSChange", "iii", playerid, upd_fps, PlayerUpdateInfo[playerid][iB_FPS]);
		PlayerUpdateInfo[playerid][iB_FPS] = upd_fps;
	}
	if(upd_drunklevel != PlayerUpdateInfo[playerid][iB_DrunkLevel])
	{
		if(upd_drunklevel != 2000)//ignore FPS updater
		{		
			CallRemoteFunction("OnPlayerDrunkLevelChange", "iii", playerid, upd_drunklevel, PlayerUpdateInfo[playerid][iB_DrunkLevel]);
			PlayerUpdateInfo[playerid][iB_DrunkLevel] = upd_drunklevel;
		}
	}
	if(upd_team != PlayerUpdateInfo[playerid][iB_Team])
	{
		CallRemoteFunction("OnPlayerTeamChange", "iii", playerid, upd_team, PlayerUpdateInfo[playerid][iB_Team]);
		PlayerUpdateInfo[playerid][iB_Team] = upd_team;
	}
	return 1;
}

GetPlayerFPS(playerid)
{
	PlayerFPSInfo[playerid][iB_LastPlayerDrunkLevel] =  GetPlayerDrunkLevel(playerid);
	if(PlayerFPSInfo[playerid][iB_LastPlayerDrunkLevel] < 100)
	{
		SetPlayerDrunkLevel(playerid, 2000);
	}
	else
	{
		if(PlayerFPSInfo[playerid][iB_NewPlayerDrunkLevel] != PlayerFPSInfo[playerid][iB_LastPlayerDrunkLevel])
		{
			PlayerFPSInfo[playerid][iB_PlayerFPS] = PlayerFPSInfo[playerid][iB_NewPlayerDrunkLevel] - PlayerFPSInfo[playerid][iB_LastPlayerDrunkLevel];
			PlayerFPSInfo[playerid][iB_NewPlayerDrunkLevel] = PlayerFPSInfo[playerid][iB_LastPlayerDrunkLevel];
			if(PlayerFPSInfo[playerid][iB_PlayerFPS] > 0 && PlayerFPSInfo[playerid][iB_PlayerFPS] < 256)
			{
 				return PlayerFPSInfo[playerid][iB_PlayerFPS] - 1;
 			}
 		}
	}
	return 0;
}

stock iB_TogglePlayerControllable(playerid, bool:toggle)
{
    CallRemoteFunction("OnPlayerFreezeStateChange", "iii", playerid, toggle, PlayerUpdateInfo[playerid][iB_Freezed]); 
    TogglePlayerControllable(playerid, toggle);
    return 1;
}

stock iB_GetSkinGender(skinid)
{
    switch(skinid)
    {
        case 74: return SKIN_GENDER_NONE;
        case 0..8, 14..30, 32..38, 42..52, 57..62, 66..68, 70..73, 78..84, 86, 94..128, 132..137, 142..144, 146, 147, 149, 153..156, 158..168, 170, 171, 173..177, 179..189, 200, 
        202..204, 206..210, 212, 213, 217, 220..223, 227..230, 234..236, 239..242, 247..250, 252..255, 258..262, 264..297, 299..305, 310, 311: return SKIN_GENDER_MALE;
        default: return SKIN_GENDER_FEMALE;
    }
    return SKIN_GENDER_NONE;
}

#if defined _ALS_TogglePlayerControllable
    #undef TogglePlayerControllable
#else
    #define _ALS_TogglePlayerControllable
#endif
<<<<<<< HEAD
#define TogglePlayerControllable iB_TogglePlayerControllable
=======
#define TogglePlayerControllable iB_TogglePlayerControllable
>>>>>>> origin/master
