#pragma once

#include "CBaseEntity.hh"
#include "Forward.hh"

class CCSWeaponInfo {
  public:
	PAD(4);
	const char* m_szClassName;
	PAD(12);
	int m_iMaxClip1;
	PAD(112);
	const char* m_szLocalizeToken;
	PAD(60);
	int m_iWeaponType;
};

NETWORKED_CLASS_INHERIT(CBaseCombatWeapon, CBaseEntity, NETWORKED_VARIABLE_DEDUCE(m_hOwner);
						NETWORKED_VARIABLE_DEDUCE(m_iClip1);

						CCSWeaponInfo * GetWeaponInfo(););