#pragma once

#include "Forward.hh"
#include <memory>
#include <vector>
#include <functional>

struct EntityCacher_t
{
	EntityCacher_t();
	inline ~EntityCacher_t(){};

	void Add(CCSPlayer *pPl);
	void Remove(CCSPlayer *pPl);
	void Reset();

	void Loop(const std::function<void(CCSPlayer *)> &runFn) const;

private:
	std::vector<CCSPlayer *> m_vecPlayers = {};
};

inline std::unique_ptr<EntityCacher_t> g_pEntityCache;