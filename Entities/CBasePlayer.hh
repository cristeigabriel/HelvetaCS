#pragma once

#include "CBaseEntity.hh"
#include "../Helpers/Helpers.hh"

NETWORKED_CLASS_INHERIT(CBasePlayer, CBaseEntity, NETWORKED_VARIABLE_DEDUCE(m_iHealth);
                        NETWORKED_VARIABLE_DEDUCE(m_fFlags);
                        PNETWORKED_VARIABLE_DEDUCE(m_szLastPlaceName);
                        NETWORKED_VARIABLE_SPECIFIER(char, m_lifeState);

                        bool Alive();

                        CBaseCombatWeapon * GetActiveWeapon(););
