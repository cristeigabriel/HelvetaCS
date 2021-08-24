#pragma once

#include "Forward.hh"

class SDK::ConVar
{
public:
	const char *GetString();
	float GetFloat();
	int GetInt();
	void SetValue(const char *szValue);
	void SetValue(float flValue);
	void SetValue(int iValue);

	PAD(0x4);

	ConVar *m_pNext;
	bool m_bRegistered;
	const char *m_szName;
	const char *m_szDetails;
	int m_fFlags;
};