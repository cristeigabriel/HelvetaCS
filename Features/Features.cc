#include "Features.hh"

#include "../Console/Console.hh"
#include "../Drawing/Drawing.hh"
#include "../Entities/CBaseCombatWeapon.hh"
#include "../Entities/CCSPlayer.hh"
#include "../Entities/Cacher.hh"
#include "../Globals/Globals.hh"
#include "../Memory.hh"
#include "../SDK/CGlobalVarsBase.hh"
#include "../SDK/CUserCmd.hh"
#include "../SDK/Constants.hh"
#include "../SDK/ILocalize.hh"
#include "../SDK/IVDebugOverlay.hh"
#include "../SDK/IVEngineClient.hh"

static inline bool ValidMoveType() {
	return !(g_pMemory->LocalPlayer()->m_nMoveType() == MOVETYPE_NOCLIP || g_pMemory->LocalPlayer()->m_nMoveType() == MOVETYPE_LADDER);
}

void Features::NoDuckDelay(SDK::CUserCmd* pCmd) {
	if (BOOL_GET(bRef, "misc.no_duck_delay"); bRef)
		pCmd->m_nButtons |= IN_BULLRUSH;
}

void Features::BunnyHop(SDK::CUserCmd* pCmd) {
	if (BOOL_GET(bRef, "misc.bunny_hop"); bRef)
		if (ValidMoveType())
			if (pCmd->m_nButtons & IN_JUMP && !(g_pMemory->LocalPlayer()->m_fFlags() & FL_ONGROUND))
				pCmd->m_nButtons &= ~IN_JUMP;
}

static bool ComputeBoundingBox(CCSPlayer* pPl, Vector_t<int>::V4& vecOut) {
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

	for (int i = 0; i < 8; ++i) {
		if (g_pMemory->m_pDebugOverlay->ScreenPosition(&rgWorld[i], &rgScreen[i]))
			return false;
	}

	float flLeft   = rgScreen[3][0];
	float flRight  = rgScreen[3][0];
	float flTop	   = rgScreen[3][1];
	float flBottom = rgScreen[3][1];

	for (int i = 1; i < 8; ++i) {
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

void Features::Visuals_t::Run(Queue_t* pQueue) {
	if (!g_pMemory->LocalPlayer())
		return;

	static const ImFont* pFont = g_pDrawing->GetFont(HASH("Terminus"));

	COLOR_GET(boxColor, "esp.box_color");
	COLOR_GET(boxFillColor, "esp.box_fill_color");
	COLOR_GET(nameColor, "esp.name_color");
	COLOR_GET(weaponColor, "esp.weapon_color");
	COLOR_GET(footstepsColor, "esp.footsteps_color");
	COLOR_GET(healthColor, "esp.health_color");
	INT_GET(iFootstepTime, "esp.footsteps_time");

	g_EntityCache.Loop([&](CCSPlayer* pPl) {
		if (!pPl->Alive())
			return;

		if (BOOL_GET(bRef, "esp.filters.enemies"); !bRef)
			if (pPl->m_iTeamNum() != g_pMemory->LocalPlayer()->m_iTeamNum())
				return;

		if (BOOL_GET(bRef, "esp.filters.teammates"); !bRef)
			if (pPl->m_iTeamNum() == g_pMemory->LocalPlayer()->m_iTeamNum())
				return;

		Animator_t& animator = this->m_umAlpha[pPl->Networkable()->Index()];
		animator.Set(!pPl->Networkable()->IsDormant(), Animation_t {3.F, Easing::OutQuart}, Animation_t {3.F, Easing::OutQuart});

		if (animator.Get() > 0.F) {
			PlayerInfo_t playerInfo {};
			if (!g_pMemory->m_pEngineClient->GetPlayerInfo(pPl->Networkable()->Index(), &playerInfo))
				return;

			Vector_t<int>::V4 vecPosition;
			if (ComputeBoundingBox(pPl, vecPosition)) {
				if (BOOL_GET(bRef, "esp.y_animation"); bRef)
					vecPosition[1] += 16 - (16 * animator.Get());

				if (BOOL_GET(bRef, "esp.box_fill"); bRef)
					pQueue->Push(std::move(std::make_shared<Rectangle_t>(vecPosition[0], vecPosition[1], vecPosition[2], vecPosition[3], boxFillColor.ModifyA(animator.Get()))));

				if (BOOL_GET(bRef, "esp.box"); bRef)
					pQueue->Push(std::move(std::make_shared<RectangleOutline_t>(vecPosition[0], vecPosition[1], vecPosition[2], vecPosition[3], boxColor.ModifyA(animator.Get()))));

				if (BOOL_GET(bRef, "esp.health"); bRef) {
					constexpr int iMaxHealth = 100;
					int iHealth				 = min(iMaxHealth, pPl->m_iHealth());

					constexpr int iWidth = 5;
					int iHeight			 = iHealth * vecPosition[3] / iMaxHealth;

					pQueue->Push(std::move(std::make_shared<Rectangle_t>(vecPosition[0] - iWidth - 2, vecPosition[1], iWidth, vecPosition[3], Color_t(0, 0, 0, 120).ModifyA(animator.Get()))));
					pQueue->Push(std::move(std::make_shared<Rectangle_t>(vecPosition[0] - iWidth - 2, vecPosition[1] + vecPosition[3] - iHeight, iWidth, iHeight, healthColor.ModifyA(animator.Get()))));
				}

				if (BOOL_GET(bRef, "esp.name"); bRef) {
					std::shared_ptr<Text_t>&& text = std::make_shared<Text_t>(vecPosition[0] + vecPosition[2] / 2, vecPosition[1] - 2, std::string {playerInfo.m_szName}, pFont, 15.F, nameColor.ModifyA(animator.Get()));
					text->m_iX -= text->m_iW / 2;
					text->m_iY -= text->m_iH;
					pQueue->Push(std::move(text));
				}

				if (BOOL_GET(bRef, "esp.weapon"); bRef)
					if (CBaseCombatWeapon* pWeapon = pPl->GetActiveWeapon(); pWeapon)
						if (CCSWeaponInfo* pWeaponInfo = pWeapon->GetWeaponInfo(); pWeaponInfo) {
							std::shared_ptr<Text_t>&& text = std::make_shared<Text_t>(vecPosition[0] + vecPosition[2] / 2, vecPosition[1] + vecPosition[3] + 2, std::string {g_pMemory->m_pVGUILocalize->Find(pWeaponInfo->m_szLocalizeToken)}, pFont, 15.F, weaponColor.ModifyA(animator.Get()));
							text->m_iX -= text->m_iW / 2;
							pQueue->Push(std::move(text));
						}
			}
		}

		if (BOOL_GET(bRef, "esp.footsteps"); bRef)
			if (this->m_umFootstepsDeq.contains(pPl->Networkable()->Index())) {
				std::deque<Footstep_t>& deqFootsteps = this->m_umFootstepsDeq[pPl->Networkable()->Index()];
				if (deqFootsteps.empty())
					return;

				std::unique_lock<std::mutex> lock(this->m_mutFootsteps);

				for (auto& entry : deqFootsteps) {
					entry.m_Animation.Set(entry.m_flFinishTime > g_pMemory->m_pGlobalVars->m_flCurTime, Animation_t {3.F, Easing::OutQuart}, Animation_t {3.F, Easing::OutQuart});

					if (entry.m_Animation.Get() > 0.F) {
						Vector_t<float>::V3 vecScreenPosition;
						if (g_pMemory->m_pDebugOverlay->ScreenPosition(&entry.m_vecPosition, &vecScreenPosition))
							continue;

						int iX			 = (int)vecScreenPosition[0] - 5;
						int iY			 = (int)vecScreenPosition[1] - 5;
						constexpr int iW = 10;
						constexpr int iH = 10;

						const Color_t& color = footstepsColor.ModifyA(entry.m_Animation.Get());
						pQueue->Push(std::move(std::make_shared<RectangleOutline_t>(iX, iY, iW, iH, color)));

						//	If within boundaries
						if (g_iMouseX >= iX && g_iMouseY >= iY && g_iMouseX <= (iX + iW) && g_iMouseY <= (iY + iH)) {
							std::shared_ptr<Text_t>&& text	   = std::make_shared<Text_t>(iX + iW / 2, iY + iH + 2, entry.m_strName + " " + std::to_string(entry.m_flFinishTime - g_pMemory->m_pGlobalVars->m_flCurTime), pFont, 15.F, color);
							std::shared_ptr<Text_t>&& location = std::make_shared<Text_t>(text->m_iX, text->m_iY + text->m_iH + 2, entry.m_strLocation + " -> " + (pPl->Networkable()->IsDormant() ? std::string {"Unknown"} : std::string {pPl->m_szLastPlaceName()}), pFont, 15.F, color);
							text->m_iX -= text->m_iW / 2;
							location->m_iX -= location->m_iW / 2;
							pQueue->Push(std::move(text));
							pQueue->Push(std::move(location));
						}
					}
				}

				if (deqFootsteps.back().m_Animation.Get() <= 0.F)
					deqFootsteps.pop_back();
			}
	});
}

void Features::Visuals_t::Reset() {
	this->m_umAlpha.clear();
	this->m_umFootstepsDeq.clear();
}

void Features::Visuals_t::AddFootstep(CBasePlayer* pPl, const Vector_t<float>::V3& vecOrigin) {
	INT_GET(iFootstepTime, "esp.footsteps_time");

	if (BOOL_GET(bRef, "esp.footsteps"); bRef && iFootstepTime > 0)
		if (pPl && vecOrigin.IsValid() && pPl->Networkable()->Index() >= 1 && pPl->Networkable()->Index() <= 64) {
			PlayerInfo_t playerInfo {};
			if (!g_pMemory->m_pEngineClient->GetPlayerInfo(pPl->Networkable()->Index(), &playerInfo))
				return;

			std::unique_lock<std::mutex> lock(this->m_mutFootsteps);

			this->m_umFootstepsDeq[pPl->Networkable()->Index()].emplace_front(vecOrigin, std::string {playerInfo.m_szName}, std::string {pPl->m_szLastPlaceName()}, g_pMemory->m_pGlobalVars->m_flCurTime + iFootstepTime, Animator_t {});
		}
}
