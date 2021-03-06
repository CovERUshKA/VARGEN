#pragma once

#include "Utils.h"
#include "../MathTools.h"

struct PlayerInfo {
	__int64 pad;
	union {
		__int64 steamID64;
		__int32 xuidLow;
		__int32 xuidHigh;
	};
	char name[128];
	int userId;
	char steamIdString[20];
	char pad1[16];
	unsigned long steamId;
	char friendsName[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};

class Engine {
public:
	constexpr auto getPlayerInfo(int entityIndex, const PlayerInfo& playerInfo) noexcept
	{
		return callVirtualMethod<bool, int, const PlayerInfo&>(this, 8, entityIndex, playerInfo);
	}

	constexpr auto getLocalPlayer() noexcept
	{
		return callVirtualMethod<int>(this, 12);
	}

	constexpr auto getViewAngles(Vector& angles) noexcept
	{
		callVirtualMethod<void, Vector&>(this, 18, angles);
	}

	constexpr auto setViewAngles(const Vector& angles) noexcept
	{
		callVirtualMethod<void, const Vector&>(this, 19, angles);
	}

	constexpr auto getMaxClients() noexcept
	{
		return callVirtualMethod<int>(this, 20);
	}

	constexpr auto isInGame() noexcept
	{
		return callVirtualMethod<bool>(this, 26);
	}

	constexpr auto isConnected() noexcept
	{
		return callVirtualMethod<bool>(this, 27);
	}

	constexpr auto IsTakingScreenshot() noexcept
	{
		return callVirtualMethod<bool>(this, 92);
	}

	using Matrix = float[4][4];

	constexpr auto worldToScreenMatrix() noexcept
	{
		return callVirtualMethod<const Matrix&>(this, 37);
	}

	constexpr auto ExecuteClientCmd(const char * cmd) noexcept
	{
		return callVirtualMethod<void, const char*>(this, 108, cmd);
	}

	virtual int GetPlayerForUserID(int userID) noexcept
	{
		return callVirtualMethod<int, const int>(this, 108, userID);
	}

	/*constexpr auto getNetworkChannel() noexcept
	{
		return callVirtualMethod<NetworkChannel*>(this, 78);
	}*/
};
