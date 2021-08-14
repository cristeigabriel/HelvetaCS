#include "CBasePlayer.hh"
#include "../Dll/Dll.hh"

bool CBasePlayer::Alive()
{
	return this && (m_iHealth() > 0) && (m_lifeState() == 0);
}

CBaseCombatWeapon *CBasePlayer::GetActiveWeapon()
{
	return Memory::VirtualCall<CBaseCombatWeapon *, 267>(this);
}