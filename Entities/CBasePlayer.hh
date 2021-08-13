#pragma once

#include "CBaseEntity.hh"

NETWORKED_CLASS_INHERIT(CBasePlayer, CBaseEntity,
						NETWORKED_VARIABLE_DEDUCE(m_iHealth);
						NETWORKED_VARIABLE_DEDUCE(m_fFlags);
						NETWORKED_VARIABLE_SPECIFIER(char, m_lifeState);
						bool Alive(););
