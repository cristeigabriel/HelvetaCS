#include "IVEngineClient.hh"

#include "../Dll/Dll.hh"

bool SDK::IVEngineClient::GetPlayerInfo(int iIdx, const PlayerInfo_t* pInfo) {
    return Memory::VirtualCall<bool, 8>(this, iIdx, pInfo);
}

bool SDK::IVEngineClient::InGame() {
    return Memory::VirtualCall<bool, 26>(this);
}

void SDK::IVEngineClient::GetViewAngles(Vector_t<float>::V3* pVec) {
    return Memory::VirtualCall<void, 18>(this, pVec);
}

void SDK::IVEngineClient::SetViewAngles(Vector_t<float>::V3* pVec) {
    return Memory::VirtualCall<void, 19>(this, pVec);
}

const char* SDK::IVEngineClient::GetGameDirectory() {
    return Memory::VirtualCall<const char*, 36>(this);
}

const char* SDK::IVEngineClient::GetLevelName() {
    return Memory::VirtualCall<const char*, 53>(this);
}

const char* SDK::IVEngineClient::GetLevelNameShort() {
    return Memory::VirtualCall<const char*, 54>(this);
}

void SDK::IVEngineClient::ExecuteClientCmd(const char* szName) {
    return Memory::VirtualCall<void, 108>(this, szName);
}