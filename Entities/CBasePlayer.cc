#include "CBasePlayer.hh"

bool CBasePlayer::Alive()
{
	return this && (m_iHealth() > 0) && (m_lifeState() == 0);
}