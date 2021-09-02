#pragma once

#include "CBasePlayer.hh"
#include "../Helpers/Helpers.hh"

NETWORKED_CLASS_INHERIT(CCSPlayer, CBasePlayer, NETWORKED_VARIABLE_DEDUCE(m_vecOrigin););