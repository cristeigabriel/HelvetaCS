#pragma once

#include "../Features.hh"

namespace Features {
    struct Misc_t {
        void NoDuckDelay(SDK::CUserCmd* pCmd);
        void BunnyHop(SDK::CUserCmd* pCmd);
    }inline g_Misc;
}  // namespace Features