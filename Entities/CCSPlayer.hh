#pragma once

#include "CBasePlayer.hh"

NETWORKED_CLASS_INHERIT(CCSPlayer, CBasePlayer,
						NETWORKED_VARIABLE_DEDUCE(m_vecOrigin););