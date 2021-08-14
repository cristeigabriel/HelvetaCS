#include "Features.hh"
#include "../Memory.hh"

#include "../SDK/Constants.hh"
#include "../SDK/CUserCmd.hh"
#include "../SDK/IVDebugOverlay.hh"
#include "../SDK/IVEngineClient.hh"

#include "../Entities/CCSPlayer.hh"
#include "../Entities/Cacher.hh"

#include "../Console/Console.hh"

#include "../Drawing/Drawing.hh"

static inline bool ValidMoveType()
{
	return !(g_pMemory->LocalPlayer()->m_nMoveType() == MOVETYPE_NOCLIP || g_pMemory->LocalPlayer()->m_nMoveType() == MOVETYPE_LADDER);
}

void Features::NoDuckDelay(SDK::CUserCmd *pCmd)
{
	if (BOOL_GET(bRef, "misc.no_duck_delay"); bRef)
		pCmd->m_nButtons |= IN_BULLRUSH;
}

void Features::BunnyHop(SDK::CUserCmd *pCmd)
{
	if (BOOL_GET(bRef, "misc.bunny_hop"); bRef)
		if (ValidMoveType())
			if (pCmd->m_nButtons & IN_JUMP && !(g_pMemory->LocalPlayer()->m_fFlags() & FL_ONGROUND))
				pCmd->m_nButtons &= ~IN_JUMP;
}

static bool ComputeBoundingBox(CCSPlayer *pPl, Vector_t<int>::V4 &vecOut)
{
	Vector_t<float>::V3 vecMins, vecMaxs;
	if (!pPl->ComputeHitboxSurroundingBox(&vecMins, &vecMaxs))
		return false;

	Vector_t<float>::V3 rgWorld[8] = {
		Vector_t<float>::V3(vecMins[0], vecMins[1], vecMins[2]),
		Vector_t<float>::V3(vecMins[0], vecMaxs[1], vecMins[2]),
		Vector_t<float>::V3(vecMaxs[0], vecMaxs[1], vecMins[2]),
		Vector_t<float>::V3(vecMaxs[0], vecMins[1], vecMins[2]),
		Vector_t<float>::V3(vecMaxs[0], vecMaxs[1], vecMaxs[2]),
		Vector_t<float>::V3(vecMins[0], vecMaxs[1], vecMaxs[2]),
		Vector_t<float>::V3(vecMins[0], vecMins[1], vecMaxs[2]),
		Vector_t<float>::V3(vecMaxs[0], vecMins[1], vecMaxs[2])};

	Vector_t<float>::V3 rgScreen[8] = {
		Vector_t<float>::V3(0.F, 0.F, 0.F),
		Vector_t<float>::V3(0.F, 0.F, 0.F),
		Vector_t<float>::V3(0.F, 0.F, 0.F),
		Vector_t<float>::V3(0.F, 0.F, 0.F),
		Vector_t<float>::V3(0.F, 0.F, 0.F),
		Vector_t<float>::V3(0.F, 0.F, 0.F),
		Vector_t<float>::V3(0.F, 0.F, 0.F),
		Vector_t<float>::V3(0.F, 0.F, 0.F)};

	for (int i = 0; i < 8; ++i)
	{
		if (g_pMemory->m_pDebugOverlay->ScreenPosition(&rgWorld[i], &rgScreen[i]))
			return false;
	}

	float flLeft = rgScreen[3][0];
	float flRight = rgScreen[3][0];
	float flTop = rgScreen[3][1];
	float flBottom = rgScreen[3][1];

	for (int i = 1; i < 8; ++i)
	{
		if (flLeft > rgScreen[i][0])
			flLeft = rgScreen[i][0];

		if (flRight < rgScreen[i][0])
			flRight = rgScreen[i][0];

		if (flBottom < rgScreen[i][1])
			flBottom = rgScreen[i][1];

		if (flTop > rgScreen[i][1])
			flTop = rgScreen[i][1];
	}

	vecOut[0] = (int)(round(flLeft));
	vecOut[1] = (int)(round(flTop));
	vecOut[2] = (int)(round(flRight - flLeft));
	vecOut[3] = (int)(round(flBottom - flTop));

	return true;
}

void Features::Visuals_t::Run(Queue_t *pQueue)
{
	if (!g_pMemory->LocalPlayer())
		return;

	static const ImFont *pFont = g_pDrawing->GetFont(HASH("Terminus"));

	COLOR_GET(boxColor, "esp.box_color");
	COLOR_GET(nameColor, "esp.name_color");

	g_pEntityCache->Loop([&](CCSPlayer *pPl)
						 {
							 if (!pPl->Alive())
								 return;

							 Animator_t &animator = this->m_umAlpha[pPl->Networkable()->Index()];
							 animator.Set(!pPl->Networkable()->IsDormant(), Animation_t{3.F, Easing::OutQuart}, Animation_t{3.F, Easing::OutQuart});

							 if (animator.Get() > 0.F)
							 {
								 PlayerInfo_t playerInfo;
								 if (!g_pMemory->m_pEngineClient->GetPlayerInfo(pPl->Networkable()->Index(), &playerInfo))
									 return;

								 Vector_t<int>::V4 vecPosition;
								 if (ComputeBoundingBox(pPl, vecPosition) && vecPosition.IsValid())
								 {
									 if (BOOL_GET(bRef, "esp.y_animation"); bRef)
										 vecPosition[1] += 16 - (16 * animator.Get());

									 if (BOOL_GET(bRef, "esp.box"); bRef)
										 pQueue->Push(std::move(std::make_shared<RectangleOutline_t>(vecPosition[0], vecPosition[1], vecPosition[2], vecPosition[3], boxColor.ModifyA(animator.Get()))));

									 if (BOOL_GET(bRef, "esp.name"); bRef)
									 {
										 std::shared_ptr<Text_t> &&text = std::make_shared<Text_t>(vecPosition[0] + vecPosition[2] / 2, vecPosition[1] - 2, std::string_view{playerInfo.m_szName}, pFont, 15.F, nameColor.ModifyA(animator.Get()));
										 text->m_iX -= text->m_iW / 2;
										 text->m_iY -= text->m_iH;
										 pQueue->Push(std::move(text));
									 }
								 }
							 }
						 });
}

void Features::Visuals_t::Reset()
{
	this->m_umAlpha.clear();
}