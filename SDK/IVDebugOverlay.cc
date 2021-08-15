#include "IVDebugOverlay.hh"
#include "../Dll/Dll.hh"

int SDK::IVDebugOverlay::ScreenPosition(const Vector_t<float>::V3 *pVecIn, Vector_t<float>::V3 *pVecOut)
{
	return Memory::VirtualCall<int, 13>(this, pVecIn, pVecOut);
}
