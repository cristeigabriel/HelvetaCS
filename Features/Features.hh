#pragma once

#include "../SDK/Forward.hh"
#include "../Entities/Forward.hh"
#include "../Animator/Animator.hh"
#include <mutex>
#include <unordered_map>

struct Queue_t;

struct Footstep_t
{
	Vector_t<float>::V3 m_vecPosition;
	float m_flFinishTime;
	int m_iAlpha;
};

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

		void AddFootstep(CBasePlayer *pPl, const Vector_t<float>::V3 &vecOrigin);

		std::unordered_map<int, Animator_t> m_umAlpha = {};

	private:
		//	Avoid parallel writing/reading during work possible issues
		std::mutex m_mutFootsteps;
		std::unordered_map<int, std::vector<Footstep_t>> m_umFootstepsVec;
	} inline g_Visuals;
}