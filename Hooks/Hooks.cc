#include "Hooks.hh"
#include "Prototypes.hh"
#include "../Memory.hh"

#include "../Console/Console.hh"

#include "../Drawing/Drawing.hh"
#include "../SDK/Constants.hh"
#include "../SDK/ClientModeShared.hh"
#include "../SDK/CGlobalVarsBase.hh"
#include "../SDK/IInputSystem.hh"

#include "../SDK/CUserCmd.hh"
#include "../SDK/IClientEntityList.hh"
#include "../SDK/ConVar.hh"

#include "../Entities/CCSPlayer.hh"
#include "../Entities/Cacher.hh"

#include "../Features/Features.hh"

#include "../Resources/Terminus.hh"

#include <WinUser.h>
#include <iomanip>

HRESULT __fastcall EndScene::Hooked(void *pThisPtr, void *pEdx, IDirect3DDevice9 *pDevice)
{
	g_pConsole->Think();
	g_pDrawing->Run([](Drawing_t *pDrawing)
					{ g_pConsole->Draw(pDrawing); });
	return Original(pThisPtr, pEdx, pDevice);
}

LRESULT WINAPI WndProc::Hooked(HWND hwndWindow, UINT u32Msg, WPARAM wParam, LPARAM lParam)
{
	g_pConsole->WndProc(u32Msg, wParam);

	//	Implementing cheat context to framework feels like a dirty thing to consider, to me
	//	So I'll place this here.
	g_pMemory->m_pInputSystem->EnableInput(!g_pConsole->m_bOpen);

	return CallWindowProcW(Original, hwndWindow, u32Msg, wParam, lParam);
}

void __cdecl VGUI_OnSplitScreenStateChanged::Hooked()
{
	Original();

	//	Here ClientModeShared m_nRootSize is updated
	g_pDrawing->UpdateIO(g_pMemory->m_pClientModeShared->m_nRootSize[0], g_pMemory->m_pClientModeShared->m_nRootSize[1]);
}

void __stdcall FrameStageNotify::Hooked(int iStage)
{
	static Queue_t &ESPQueue = g_pDrawing->GetQueue(HASH("ESP"));
	ESPQueue.Run([&](Queue_t *pQueue)
				 { Features::g_Visuals.Run(pQueue); });
	Original(iStage);
}

void __fastcall OnAddEntity::Hooked(void *pThisPtr, void *pEdx, void *pArg1, int iHandle)
{
	CCSPlayer *pPl = g_pMemory->m_pEntityList->GetFromHandle<CCSPlayer *>(iHandle);
	g_pEntityCache->Add(pPl);
	Original(pThisPtr, pEdx, pArg1, iHandle);
}

void __fastcall OnRemoveEntity::Hooked(void *pThisPtr, void *pEdx, void *pArg1, int iHandle)
{
	CCSPlayer *pPl = g_pMemory->m_pEntityList->GetFromHandle<CCSPlayer *>(iHandle);
	g_pEntityCache->Remove(pPl);
	Original(pThisPtr, pEdx, pArg1, iHandle);
}

void __stdcall LevelInitPreEntity::Hooked(const char *szName)
{
	float flTickrate = 1.F / g_pMemory->m_pGlobalVars->m_flIntervalPerTick;

	g_pMemory->m_Cvars.cl_cmdrate->SetValue(flTickrate);
	g_pMemory->m_Cvars.cl_updaterate->SetValue(flTickrate);

	g_pEntityCache->Reset();
	Features::g_Visuals.Reset();
	Original(szName);
}

void __cdecl LevelInitPostEntity::Hooked()
{
	Original();
}

bool __stdcall CreateMove::Hooked(float flSampleTime, SDK::CUserCmd *pCmd)
{
	if (!flSampleTime || !pCmd || (pCmd && !pCmd->m_iCommandNumber) || !g_pMemory->LocalPlayer() || (g_pMemory->LocalPlayer() && !g_pMemory->LocalPlayer()->Alive()))
	{
		return Original(flSampleTime, pCmd);
	}

	Original(flSampleTime, pCmd);

	Features::NoDuckDelay(pCmd);
	Features::BunnyHop(pCmd);

	pCmd->m_vecViewAngles.NormalizeAngle();
	pCmd->m_vecViewAngles.ClampAngle();

	return false;
}

/**
 * @brief Utility Macros
 * 
 */
//	========================================================================================================================================
#define HOOK_PAD(x, pad, ...)                                 \
	static_assert(sizeof(#x) <= 40);                          \
	static void *CONCAT(p, x) = __VA_ARGS__.Get<void *>(pad); \
	g_pMemory->ApplyHook<x>(CONCAT(p, x));                    \
	LOG(#x << std::setw(Helveta::DataHolder_t<40 - Helveta::detail::strlen(#x)>::value) << "->" << std::setw(10) << std::hex << CONCAT(p, x))

#define HOOK(x, ...) HOOK_PAD(x, 0, __VA_ARGS__)
//	========================================================================================================================================

/**
 * @brief Bootstrap everything after memory initialization with it's contained data.
 * 
 */
void Hooks::Bootstrap()
{
	hwndWindow = FindWindowA("Valve001", nullptr);

	//	Set up drawing manager.
	//	ClientModeShared is relied on, because, from superficial observation, it appears that
	//	the Device returned viewport will often be wrong.
	g_pDrawing = std::make_unique<Drawing_t>(g_pMemory->m_pDevice, g_pMemory->m_pClientModeShared->m_nRootSize[0], g_pMemory->m_pClientModeShared->m_nRootSize[1]);
	{
		g_pDrawing->AddQueueLocked(HASH("ESP"), false);
		g_pDrawing->AddFont(HASH("Terminus"), Terminus_compressed_data, Terminus_compressed_size, 17.F);
	}

	//	Menu, config, callbacks.
	g_pConsole = std::make_unique<Console_t>("config.helveta");
	{
		g_pConsole->AddIdentifier("misc.no_duck_delay", false);
		g_pConsole->AddIdentifier("misc.bunny_hop", false);

		g_pConsole->AddCallback("misc.unlock_convar", [](Console_t *pConsole)
								{
									static SDK::ConVar *pFirstCVar = *g_pMemory->m_Client.FindPattern(STB("6A 00 51 C7 04 24 ? ? ? ? 6A 00 51 C7 04 24 ? ? ? ? B9 ? ? ? ?")).FollowUntil(0xB9, true).Get<SDK::ConVar **>(1);
									if (!pFirstCVar)
										return false;

									int iCount = 0;
									for (SDK::ConVar *pCVar = pFirstCVar; pCVar; pCVar = pCVar->m_pNext)
									{
										//	(1 << 1) = 2 = FCVAR_DEVELOPMENTONLY
										if (pCVar->m_nFlags & 2)
										{
											pCVar->m_nFlags &= ~2;
											++iCount;
										}
									}

									const std::string &strPresentationString = std::string("Unlocked " + std::to_string(iCount) + " Convars.");
									pConsole->WriteToBuffer(strPresentationString);
									return true;
								});

		g_pConsole->AddIdentifier("esp.box", false);

		g_pConsole->AddCallback("config.save", [](Console_t *pConsole)
								{
									pConsole->SaveConfig();
									return true;
								});

		g_pConsole->AddCallback("config.load", [](Console_t *pConsole)
								{ return pConsole->LoadConfig(); });

		g_pConsole->AddCallback("help", [](Console_t *pConsole)
								{
									if (pConsole->m_vecCollection.empty())
										return false;

									pConsole->WriteToBuffer("- The following represents all the available help material");
									for (const auto &[first, second] : pConsole->m_vecCollection)
									{
										const std::string &strPresentationString = std::string(first.first + ": " + (second.first ? "Callback (command)" : "Identifier (value)"));
										pConsole->WriteToBuffer(strPresentationString);
									}

									return true;
								});
	}

	g_pNetvars = std::make_unique<Netvars_t>(g_pMemory->m_pClientClassHead);

	g_pEntityCache = std::make_unique<EntityCacher_t>();

	//	Finally, hooks.
	WndProc::Original = (WndProc::Fn_t *)SetWindowLongA(hwndWindow, GWL_WNDPROC, (LONG_PTR)WndProc::Hooked);
	HOOK(EndScene, Memory::Pointer_t(Memory::VirtualFunction<42>(g_pMemory->m_pDevice)));
	HOOK(VGUI_OnSplitScreenStateChanged, g_pMemory->m_Client.FindString<"fov_desired", true>(0xA3).FollowUntil(0x55, false));
	HOOK(FrameStageNotify, g_pMemory->m_Client.FindString<"Setting fallback player %s as local player\n", false>().FollowUntil(0x55, false));
	HOOK(OnAddEntity, g_pMemory->m_Client.FindPattern(STB("55 8B EC 51 8B 45 0C 53 56 8B F1 57")));
	HOOK(OnRemoveEntity, g_pMemory->m_Client.FindPattern(STB("55 8B EC 51 8B 45 0C 53 8B D9 56 57 83 F8 FF 75 07")));
	static const Memory::Pointer_t &levelInitPrePost = g_pMemory->m_Client.FindString<"(mapname)", false>();
	HOOK(LevelInitPreEntity, levelInitPrePost.FollowUntil(0x55, false));
	HOOK(LevelInitPostEntity, levelInitPrePost.FollowUntil(0x55, true));
	HOOK(CreateMove, g_pMemory->m_Client.FindPattern(STB("55 8B EC 8B 0D ? ? ? ? 85 C9 75 06")));
}