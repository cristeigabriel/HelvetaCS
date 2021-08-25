#include "CBaseEntity.hh"
#include "../Memory.hh"

IClientNetworkable *CBaseEntity::Networkable()
{
	return (IClientNetworkable *)((uintptr_t)this + 0x8);
}

bool CBaseEntity::ComputeHitboxSurroundingBox(Vector_t<float>::V3 *pVecMins, Vector_t<float>::V3 *pVecMaxs)
{
	static const Memory::Pointer_t &computeHitboxSurroundingBox = g_pMemory->m_Client.FindPattern(STB("55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 83 BE ? ? ? ? ? 75 14 8B 46 04 8D 4E 04 FF 50 20"));
	using ComputeHitboxSurroundingBox_t = bool(__thiscall *)(CBaseEntity *, Vector_t<float>::V3 *, Vector_t<float>::V3 *);
	return computeHitboxSurroundingBox.Get<ComputeHitboxSurroundingBox_t>()(this, pVecMins, pVecMaxs);
}

Vector_t<float>::V3 &CBaseEntity::GetAbsOrigin()
{
	return Memory::VirtualCall<Vector_t<float>::V3 &, 10>(this);
}

void CBaseEntity::SetAbsOrigin(const Vector_t<float>::V3 &vecOrigin)
{
	static const Memory::Pointer_t &setAbsOrigin = g_pMemory->m_Client.FindPattern(STB("55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D 08 F3 0F 10 07 0F 2E 86"));
	using SetAbsOrigin_t = void(__thiscall *)(CBaseEntity *, const Vector_t<float>::V3 &);
	setAbsOrigin.Get<SetAbsOrigin_t>()(this, vecOrigin);
}