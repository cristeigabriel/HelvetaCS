#include "CBaseCombatWeapon.hh"

#include "../Dll/Dll.hh"

CCSWeaponInfo* CBaseCombatWeapon::GetWeaponInfo() {
	return Memory::VirtualCall<CCSWeaponInfo*, 461>(this);
}