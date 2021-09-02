#pragma once

#include <memory>
#include <type_traits>
#include <unordered_map>

#include "../Helpers/Types.hh"
#include "../Helpers/Util.hh"

#include "../SDK/Forward.hh"

struct Netvars_t {
	inline Netvars_t() = default;
	Netvars_t(SDK::ClientClass* pClassHead);
	inline ~Netvars_t() {};

	DEFAULT_COPY_MOVE_INIT_ASSIGN(Netvars_t);

	ptrdiff_t Get(Hash_t hClass, Hash_t hProperty) const;

  private:
	void Dump(const char* szNetworkName, SDK::RecvTable* pTable, ptrdiff_t ptrdiffOffset = 0);

	std::unordered_map<Hash_t, std::unordered_map<Hash_t, ptrdiff_t>> m_umPtrdiffs;
};

/**
 * @brief Utility Macros
 * 
 * NETWORKED_CLASS will store a private hash of the class name to use when accessing
 * netvars map, as we use Networked names.
 * 
 * @todo: Either wait with this until there's an implementation of some sort of preprocessor macro
 * along the lines of '__CLASS__', should be self explainatory why,
 * or write a sanitized type information class (__FUNCTION__ has C style type specifiers for structs, classes, etc),
 * (source_location::current().function_name() returns _func_) etc
 * 
 * NETWORKED_VARIABLE_DEDUCE(_[SI/NN]) will call to the Class Member Hungarian Notation Type Parser
 * to deduce NetVar type in shorthand:
 * 
 * Drawbacks:
 * 	-	Valve is inconsistent with Hungarian notation, as you can observe in the parser comments there,
 * I've noted that they use many notations interchangibly and differently (i.e. n for nibbles AND for count,
 * f for flag/bitmask and for float despite 'fl' existing, etc...)
 * 
 * For concrete examples of the aforementioned refer to Helpers.hh directly
 * 
 * Fix approach:
 * 		- Implemented ..._SI and ..._NN which respectively make integers shorts (for m_iItemDefinitionIndex cases), or make the 'n'
 * notation return a nibble (uint8_t)
 * 
 * In any case:
 * 		- Implemented direct type specifier ability for Netvars.
 * 
 * Extra dumb cases where there's nothing we can (immediately) do:
 * 		- Implemented OFFSET method
 * 
 */
//	========================================================================================================================================
#define NETWORKED_CLASS(x, ...) \
	class x { \
		constexpr static Hash_t hHash = HASH(#x); \
\
	  public: \
		__VA_ARGS__ \
	};

#define NETWORKED_CLASS_INHERIT(x, inherit, ...) \
	class x: public inherit { \
		constexpr static Hash_t hHash = HASH(#x); \
\
	  public: \
		__VA_ARGS__ \
	};

#define NETWORKED_VARIABLE_DEDUCE(x) \
	GET_TYPE(#x) & x() { \
		static ptrdiff_t CONCAT(x, _ptrdiff) = g_Netvars.Get(hHash, HASH(#x)); \
		return *(GET_TYPE(#x)*)((uintptr_t)this + CONCAT(x, _ptrdiff)); \
	}

#define PNETWORKED_VARIABLE_DEDUCE(x) \
	GET_TYPE(#x) \
	x() { \
		static ptrdiff_t CONCAT(x, _ptrdiff) = g_Netvars.Get(hHash, HASH(#x)); \
		return (GET_TYPE(#x))((uintptr_t)this + CONCAT(x, _ptrdiff)); \
	}

#define NETWORKED_VARIABLE_DEDUCE_NN(x) \
	GET_TYPE_NN(#x) & x() { \
		static ptrdiff_t CONCAT(x, _ptrdiff) = g_Netvars.Get(hHash, HASH(#x)); \
		return *(GET_TYPE_NN(#x)*)((uintptr_t)this + CONCAT(x, _ptrdiff)); \
	}

#define NETWORKED_VARIABLE_DEDUCE_SI(x) \
	GET_TYPE_SI(#x) & x() { \
		static ptrdiff_t CONCAT(x, _ptrdiff) = g_Netvars.Get(HASH(hHash, HASH(#x)); \
		return *(GET_TYPE_SI(#x) *)((uintptr_t)this + CONCAT(x, _ptrdiff)); \
	}

#define NETWORKED_VARIABLE_SPECIFIER(t, x) \
	t& x() { \
		static ptrdiff_t CONCAT(x, _ptrdiff) = g_Netvars.Get(hHash, HASH(#x)); \
		return *(t*)((uintptr_t)this + CONCAT(x, _ptrdiff)); \
	}

#define OFFSET(t, n, x) \
	t& n() { \
		static ptrdiff_t CONCAT(n, _ptrdiff) = x; \
		return *(t*)((uintptr_t)this + CONCAT(n, _ptrdiff)); \
	}
//	========================================================================================================================================

inline Netvars_t g_Netvars;