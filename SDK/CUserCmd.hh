#pragma once

#include "Forward.hh"
#include "../Helpers/Helpers.hh"

class SDK::CUserCmd
{
	PAD(4);

public:
	int m_iCommandNumber;
	int m_iTickCount;

	Vector_t<float>::V3 m_vecViewAngles;
	Vector_t<float>::V3 m_vecAimDirection;

	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;

	int m_nButtons;

	char m_nImpulse;

	int m_nWeaponSelect;
	int m_nWeaponSubtype;

	int m_nRandomSeed;

	short m_iMouseDx;
	short m_iMouseDy;

	bool m_bHasBeenPredicted;
};