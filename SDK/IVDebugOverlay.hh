#pragma once

#include "Forward.hh"
#include "../Helpers/Helpers.hh"

class SDK::IVDebugOverlay
{
public:
	int ScreenPosition(Vector_t<float>::V3 *pVecIn, Vector_t<float>::V3 *pVecOut);
};