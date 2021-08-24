#pragma once

#include "Forward.hh"
#include "../Helpers/Helpers.hh"
#include <cinttypes>

struct PlayerInfo_t
{
	uint64_t m_nVersion;
	union
	{
		uint64_t m_nSteamID;
		struct
		{
			uint32_t m_nXuidLow;
			uint32_t m_nXuidHigh;
		};
	};
	char m_szName[128];
	int m_nUserId;
	char m_szGUid[33];
	uint32_t m_nFriendsId;
	char m_szFriendsName[128];
	bool m_bFakePlayer;
	bool m_bIsHltv;
	int m_iCustomFiles[4];
	uint8_t m_nFilesDownloaded;
};

class SDK::IVEngineClient
{
public:
	bool GetPlayerInfo(int iIdx, const PlayerInfo_t *pInfo);
	bool InGame();
	void SetViewAngles(Vector_t<float>::V3 *pVecAngles);
	void GetViewAngles(Vector_t<float>::V3 *pVecAngles);
	const char *GetGameDirectory();
	const char *GetLevelName();
	const char *GetLevelNameShort();
	void ExecuteClientCmd(const char *szName);
};