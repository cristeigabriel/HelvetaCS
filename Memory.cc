#include "Memory.hh"
#include "Helpers/Helpers.hh"
#include <iomanip>

/**
 * @brief Utility macros 
 * 
 */
//	========================================================================================================================================
#define SET(x, ...)                  \
	static_assert(sizeof(#x) <= 40); \
	x = __VA_ARGS__;                 \
	LOG(#x << std::setw(Helveta::DataHolder_t<40 - Helveta::detail::strlen(#x)>::value) << "->" << std::setw(10) << std::hex << (uintptr_t)x)

#define ADDRSET_PAD(x, type, pad, ...) SET(x, __VA_ARGS__.Get<type>(pad))
#define ADDRSET(x, type, ...) ADDRSET_PAD(x, type, 0, __VA_ARGS__)
//	========================================================================================================================================

template <Helveta::CompileTimeString_t Str, bool ServerBounded = false>
static SDK::ConVar *FindConVar(const Memory::Dll_t &module)
{
	uintptr_t uptrString = module.FindPattern(CAST_ARRAY(int, FOLD_LITERALS_INTO_ARRAY(Str.m_szData)), 0u, HASH(".rdata")).Get();
	const std::array<int, 4> &rgReversedString = RT_CAST_ARRAY(int, Helveta::detail::ToArray32bit(Helveta::detail::EndiannessSwap32bit(uptrString)));

	Memory::Pointer_t xref = module.FindPattern(rgReversedString);

	constexpr int nPad = (ServerBounded ? -6 : 4);
	constexpr uint8_t u8Indice = (ServerBounded ? 0x68 : 0xE8);
	size_t iCount = 0u;

	//  Get next xref unless condition is met.
	while (*xref.Get<uint8_t *>(nPad) != u8Indice)
	{
		++iCount;
		xref = module.FindPattern(rgReversedString, iCount);
	}

	assert(xref.Get());

	//	Avoid memory conflicts
	const Memory::Pointer_t &baseResult = xref.FollowUntil(0xC7, ServerBounded);

	return *(ServerBounded ? baseResult : baseResult.FollowUntil(0xB9, true)).Get<SDK::ConVar **>(1 + (int)ServerBounded);
}

Memory_t::Memory_t() : m_Client("client.dll"), m_Engine("engine.dll"), m_GameOverlayRenderer("gameoverlayrenderer.dll"), m_ShaderApiDx9("shaderapidx9.dll")
{
	ADDRSET(this->m_HookFn, Hook_t, this->m_GameOverlayRenderer.FindString<"Couldn't get trampoline region lock, will continue possibly unsafely.\n", true>(0xA1, 12).FollowUntil(0x55, false));

	ADDRSET_PAD(this->m_pDevice, IDirect3DDevice9 ***, 1, **this->m_ShaderApiDx9.FindPattern(STB("A1 ? ? ? ? 50 8B 08 FF 51 0C")));

	ADDRSET_PAD(this->m_pClientClassHead, SDK::ClientClass ***, 1, **this->m_Client.FindString<"Unknown entity class received in ProcessSpottedEntityUpdate.\n", true>(0xFF).FollowUntil(0xA1, false));
	ADDRSET_PAD(this->m_pGlobalVars, SDK::CGlobalVarsBase **, 1, *this->m_Engine.FindString<"VClientDllSharedAppSystems001", false>().FollowUntil(0x68, false).FollowUntil(0x68, false));
	ADDRSET_PAD(this->m_pClientModeShared, SDK::ClientModeShared ***, 1, **this->m_Client.FindString<"MenuOpen", true>(0x8B).FollowUntil(0x0D, false));
	ADDRSET_PAD(this->m_pEntityList, SDK::IClientEntityList **, 1, *this->m_Client.FindPattern(STB("B9 ? ? ? ? FF 10 85 C0 74 0B 8B 10 8B C8 FF 74 F7 04 FF 52 14")));
	ADDRSET_PAD(this->m_pInputSystem, SDK::IInputSystem ***, 1, **this->m_Client.FindString<"InputSystemVersion001", true>().FollowUntil(0xA3, true));
	ADDRSET_PAD(this->m_pDebugOverlay, SDK::IVDebugOverlay ***, 1, **this->m_Client.FindString<"VDebugOverlay004", false>().FollowUntil(0xA3, true));
	ADDRSET_PAD(this->m_pEngineClient, SDK::IVEngineClient ***, 1, **this->m_Client.FindString<"VEngineClient014", true>(0xFF).FollowUntil(0xA3, true));
	ADDRSET_PAD(this->m_pVGUILocalize, SDK::ILocalize ***, 1, **this->m_Engine.FindString<"g_pVGuiLocalize->AddFile", false>().FollowUntil(0x0D, true));

	SET(this->m_CVars.cl_updaterate, FindConVar<"cl_updaterate", true>(this->m_Engine));
	SET(this->m_CVars.cl_cmdrate, FindConVar<"cl_cmdrate", true>(this->m_Engine));

	ADDRSET_PAD(this->m_ppLocal, CCSPlayer ***, 1, *this->m_Client.FindString<"No local player %d after full frame update\n", false>().FollowUntil(0x3D, false));
}

CCSPlayer *Memory_t::LocalPlayer() const
{
	return *this->m_ppLocal;
}