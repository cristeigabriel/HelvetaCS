#pragma once

/**
 * @brief Utility Macros
 * 
 */
//	========================================================================================================================================
#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)
#define PAD(x)                          \
private:                                \
	char CONCAT(__pad, __COUNTER__)[x]; \
                                        \
public:
//	========================================================================================================================================

namespace SDK
{
	//	The following structures will follow the Valve decided name for ease
	//	in research.
	//	Members anywho may, or not, be an exception to the rule, given inconsistencies that are
	//	painfully apparent even in superficial scours.

	class CUserCmd;

	class DVariant;
	class CRecvProxyData;
	struct RecvTable;
	struct RecvProp;
	class ClientClass;

	class CGlobalVarsBase;
	struct ClientModeShared;
	class IClientEntityList;
	class ConVar;
	class IInputSystem;
	class IVDebugOverlay;
	class IVEngineClient;
	
	//	Likewise with typedefs.
	using CreateClientClass = void *(__cdecl *)(int, int);
	using CreateEvent = void *(__cdecl *)();
	using RecvVarProxy = void(__cdecl *)(const CRecvProxyData *, void *, void *);
	using ArrayLengthRecvProxy = void(__cdecl *)(void *, int, int);
	using DataTableRecvVarProxy = void(__cdecl *)(const RecvProp *, void **, void *, int);
}
