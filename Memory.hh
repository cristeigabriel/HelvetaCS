#pragma once

#include "Dll/Dll.hh"
#include "SDK/Forward.hh"
#include "Entities/Forward.hh"
#include <memory>
#include <d3d9.h>
#undef NDEBUG
#include <assert.h>

extern inline const char *GetVftTableRawName(void *pVftPtr);

struct CVars_t
{
	SDK::ConVar *cl_updaterate;
	SDK::ConVar *cl_cmdrate;
};

struct Memory_t
{
	Memory_t();

	Memory::Dll_t m_Client;
	Memory::Dll_t m_Engine;
	Memory::Dll_t m_GameOverlayRenderer;
	Memory::Dll_t m_ShaderApiDx9;

	SDK::ClientClass *m_pClientClassHead;

	IDirect3DDevice9 *m_pDevice;
	SDK::CGlobalVarsBase *m_pGlobalVars;
	SDK::ClientModeShared *m_pClientModeShared;
	SDK::IClientEntityList *m_pEntityList;
	SDK::IInputSystem *m_pInputSystem;
	SDK::IVDebugOverlay *m_pDebugOverlay;
	SDK::IVEngineClient *m_pEngineClient;
	SDK::ILocalize *m_pVGUILocalize;

	CVars_t m_CVars;

	CCSPlayer *LocalPlayer() const;

	template <typename Prototype>
	void ApplyHook(void *pAddress) const;

private:
	CCSPlayer **m_ppLocal;
	using Hook_t = bool(__cdecl *)(void *, void *, void *, int);
	Hook_t m_HookFn;
};

template <typename Prototype>
void Memory_t::ApplyHook(void *pAddress) const
{
	assert(this->m_HookFn(pAddress, Prototype::Hooked, &Prototype::Original, 0));
}

inline std::unique_ptr<Memory_t> g_pMemory;