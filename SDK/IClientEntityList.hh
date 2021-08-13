#pragma once

#include "Forward.hh"
#include "../Dll/Dll.hh"

class SDK::IClientEntityList
{
public:
	//	Can't move to .cc due to templates presence.
	//	Anyhow, this shouldn't change any soon.
	template <class T>
	T Get(int iIdx)
	{
		return Memory::VirtualCall<T, 3>(this, iIdx);
	}

	template <class T>
	T GetFromHandle(int iIdx)
	{
		return Memory::VirtualCall<T, 4>(this, iIdx);
	}
};