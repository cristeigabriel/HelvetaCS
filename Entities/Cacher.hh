#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Forward.hh"

struct EntityCacher_t {
	EntityCacher_t();
	inline ~EntityCacher_t() {};

	void Add(CCSPlayer* pPl);
	void Remove(CCSPlayer* pPl);
	void Reset();

	void Loop(const std::function<void(CCSPlayer*)>& runFn) const;

  private:
	std::vector<CCSPlayer*> m_vecPlayers = {};
};

inline EntityCacher_t g_EntityCache;