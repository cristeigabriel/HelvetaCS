#include "ILocalize.hh"

#include "../Dll/Dll.hh"

const char* SDK::ILocalize::Find(const char* szLocalizationToken) {
	return Memory::VirtualCall<const char*, 47>(this, szLocalizationToken);
}