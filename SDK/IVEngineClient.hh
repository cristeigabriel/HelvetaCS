#pragma once

#include "Forward.hh"
#include "../Helpers/Helpers.hh"
#include <cinttypes>

struct PlayerInfo_t
{
	uint64_t m_u64Version;
	union
	{
		uint64_t m_u64SteamID;
		struct
		{
			uint32_t m_u32XuidLow;
			uint32_t m_u32XuidHigh;
		};
	};
	char m_szName[128];
	int m_iUserId;
	char m_szGUid[33];
	uint32_t m_u32FriendsId;
	char m_szFriendsName[128];
	bool m_bFakePlayer;
	bool m_bIsHltv;
	int m_iCustomFiles[4];
	uint8_t m_u8FilesDownloaded;
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