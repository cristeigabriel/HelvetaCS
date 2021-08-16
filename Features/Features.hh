#pragma once

#include "../SDK/Forward.hh"
#include "../Entities/Forward.hh"
#include "../Animator/Animator.hh"
#include <mutex>
#include <unordered_map>
#include <deque>

struct Queue_t;

struct Footstep_t
{
	Vector_t<float>::V3 m_vecPosition;
	std::string m_strName;
	std::string m_strLocation;
	float m_flFinishTime;
	Animator_t m_Animation;
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
		std::unordered_map<int, std::deque<Footstep_t>> m_umFootstepsDeq;
	} inline g_Visuals;
}