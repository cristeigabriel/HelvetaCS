#include "Misc.hh"

#include "../../Console/Console.hh"
#include "../../Drawing/Drawing.hh"
#include "../../Entities/CBaseCombatWeapon.hh"
#include "../../Entities/CCSPlayer.hh"
#include "../../Entities/Cacher.hh"
#include "../../Globals/Globals.hh"
#include "../../Memory.hh"
#include "../../SDK/CGlobalVarsBase.hh"
#include "../../SDK/CUserCmd.hh"
#include "../../SDK/Constants.hh"
#include "../../SDK/ILocalize.hh"
#include "../../SDK/IVDebugOverlay.hh"
#include "../../SDK/IVEngineClient.hh"

static inline bool ValidMoveType() {
    return !(g_pMemory->LocalPlayer()->m_nMoveType() == MOVETYPE_NOCLIP || g_pMemory->LocalPlayer()->m_nMoveType() == MOVETYPE_LADDER);
}

void Features::Misc_t::NoDuckDelay(SDK::CUserCmd* pCmd) {
    if (BOOL_GET(bRef, "misc.no_duck_delay"); bRef)
        pCmd->m_nButtons |= IN_BULLRUSH;
}

void Features::Misc_t::BunnyHop(SDK::CUserCmd* pCmd) {
    if (BOOL_GET(bRef, "misc.bunny_hop"); bRef)
        if (ValidMoveType())
            if (pCmd->m_nButtons & IN_JUMP && !(g_pMemory->LocalPlayer()->m_fFlags() & FL_ONGROUND))
                pCmd->m_nButtons &= ~IN_JUMP;
}