#include "ConVar.hh"
#include "../Dll/Dll.hh"

const char *SDK::ConVar::GetString()
{
	return Memory::VirtualCall<const char *, 11>(this);
}

float SDK::ConVar::GetFloat()
{
	return Memory::VirtualCall<float, 12>(this);
}

int SDK::ConVar::GetInt()
{
	return Memory::VirtualCall<int, 13>(this);
}

void SDK::ConVar::SetValue(const char *szValue)
{
	return Memory::VirtualCall<void, 14>(this, szValue);
}

void SDK::ConVar::SetValue(float flValue)
{
	return Memory::VirtualCall<void, 15>(this, flValue);
}

void SDK::ConVar::SetValue(int iValue)
{
	return Memory::VirtualCall<void, 16>(this, iValue);
}