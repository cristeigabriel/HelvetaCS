#pragma once

#include "../SDK/Forward.hh"
#include "../Entities/Forward.hh"
#include "../Animator/Animator.hh"
#include <unordered_map>

struct Queue_t;
namespace Features
{
	extern inline void NoDuckDelay(SDK::CUserCmd *pCmd);
	extern inline void BunnyHop(SDK::CUserCmd *pCmd);

	struct Visuals_t
	{
		Visuals_t() = default;
		inline ~Visuals_t(){};

		void Run(Queue_t *);
		void Reset();

		std::unordered_map<int, Animator_t> m_umAlpha = {};
	} inline g_Visuals;
}