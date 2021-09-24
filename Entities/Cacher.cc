#include "Cacher.hh"

#include "../Memory.hh"
#include "CCSPlayer.hh"

EntityCacher_t::EntityCacher_t() {
    this->m_vecPlayers.reserve(64);
}

void EntityCacher_t::Add(CCSPlayer* pPl) {
    if (pPl) {
        const auto itFound = std::ranges::find(this->m_vecPlayers, pPl);
        if (pPl != g_pMemory->LocalPlayer() && itFound == this->m_vecPlayers.cend() && pPl->Networkable()->Index() >= 1 && pPl->Networkable()->Index() <= 64)
            this->m_vecPlayers.push_back(pPl);
    }
}

void EntityCacher_t::Remove(CCSPlayer* pPl) {
    if (pPl) {
        const auto itFound = std::ranges::find(this->m_vecPlayers, pPl);
        if (itFound != this->m_vecPlayers.cend())
            this->m_vecPlayers.erase(itFound);
    }
}

void EntityCacher_t::Reset() {
    this->m_vecPlayers.clear();
}

void EntityCacher_t::Loop(const std::function<void(CCSPlayer*)>& runFn) const {
    for (const auto pPl : this->m_vecPlayers) {
        if (pPl && pPl != g_pMemory->LocalPlayer())
            runFn(pPl);
    }
}