#pragma once

#include "../Entities/Forward.hh"
#include "../SDK/Forward.hh"
#include <type_traits>
#include <d3d9.h>

/**
 * @brief Utility macro
 * 
 */
//	========================================================================================================================================
#define PROTOTYPE(name, type)                     \
	struct name                                   \
	{                                             \
		using Fn_t = std::remove_pointer_t<type>; \
		static Fn_t Hooked;                       \
		static Fn_t *Original;                    \
	};                                            \
	inline name::Fn_t *name::Original;
//	========================================================================================================================================

/**
 * @brief Hook prototypes
 * 
 */
//	========================================================================================================================================
PROTOTYPE(EndScene, HRESULT(__fastcall *)(void *, void *, IDirect3DDevice9 *));
PROTOTYPE(WndProc, LRESULT(WINAPI *)(HWND, UINT, WPARAM, LPARAM));
PROTOTYPE(VGUI_OnSplitScreenStateChanged, void(__cdecl *)());
PROTOTYPE(FrameStageNotify, void(__stdcall *)(int));
PROTOTYPE(OnAddEntity, void(__fastcall *)(void *, void *, void *, int));
PROTOTYPE(OnRemoveEntity, void(__fastcall *)(void *, void *, void *, int));
PROTOTYPE(LevelInitPreEntity, void(__stdcall *)(const char *));
PROTOTYPE(LevelInitPostEntity, void(__cdecl *)());
PROTOTYPE(CreateMove, bool(__stdcall *)(float, SDK::CUserCmd *));
PROTOTYPE(PlayStepSound, void(__fastcall *)(CCSPlayer *, void *, Vector_t<float>::V3 &, void *, float, bool, void *));
PROTOTYPE(GlowEffectSpectator, bool(__cdecl *)(CCSPlayer *, CCSPlayer *, int, Vector_t<float>::V3 &, float &, float &, float &, float &, bool &));
//	========================================================================================================================================
