#include <a_samp>
#include <ibranch>

main();
public OnGameModeInit()
{
	print("Gamemode started!");
	return 1;
}

public OnPlayerConnect(playerid)
{
	#if defined iBranch_OnPlayerConnect
		iBranch_OnPlayerConnect(playerid);
	#endif
	return 1;
}
public OnPlayerWeaponSlotChange(playerid, slotid, new_weapon, oldweapon)
{
	new string[64];
	format(string, 64, "Weapon slot %d changed from weapon %d to weapon %d", slotid, oldweapon, new_weapon);
	SendClientMessage(playerid, -1, string);
	return 1;
}

public OnPlayerSkinChange(playerid, newskin, oldskin)
{
	new string[64];
	format(string, 64, "Skin changed from %d to %d", oldskin, newskin);
	SendClientMessage(playerid, -1, string);
	return 1;
}

public OnPlayerMoneyChange(playerid, newmoney, oldmoney)
{
	new string[64];
	format(string, 64, "Money changed from %d to %d", oldmoney, newmoney);
	SendClientMessage(playerid, -1, string);
	return 1;
}

public OnPlayerWantedLevelChange(playerid, newlevel, oldlevel)
{
	new string[64];
	format(string, 64, "Wanted level changed from %d to %d", oldlevel, newlevel);
	SendClientMessage(playerid, -1, string);
	return 1;
}