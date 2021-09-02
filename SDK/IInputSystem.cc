#include "IInputSystem.hh"

#include "../Dll/Dll.hh"

void SDK::IInputSystem::EnableInput(bool bState) {
	return Memory::VirtualCall<void, 11>(this, bState);
}