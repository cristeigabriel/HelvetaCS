#pragma once

#include "../Helpers/Helpers.hh"
#include <Windows.h>
#include <cstdint>
#include <array>
#include <unordered_map>
#undef NDEBUG
#include <assert.h>

namespace Memory
{
	/**
	 * @brief Virtual Table Methods
	 * 
	 */
	template <size_t Index>
	constexpr auto VirtualFunction(void *pInstance)
	{
		return ((*(uintptr_t **)(pInstance))[Index]);
	}

	template <typename T, size_t Index, typename... Args>
	constexpr T VirtualCall(void *pInstance, Args... args)
	{
		using Fn = T(__thiscall *)(void *, decltype(args)...);
		return ((Fn)(VirtualFunction<Index>(pInstance)))(pInstance, args...);
	}

	template <typename T, size_t Index, typename... Args>
	constexpr T VirtualCallCdecl(void *pInstance, Args... args)
	{
		using Fn = T(__cdecl *)(void *, decltype(args)...);
		return ((Fn)(VirtualFunction<Index>(pInstance)))(pInstance, args...);
	}

	/**
	 * @brief Pointer holder
	 * 
	 */
	struct Pointer_t
	{
		Pointer_t() = default;
		Pointer_t(uintptr_t uptrAddress);
		inline ~Pointer_t(){};

		const Pointer_t FollowUntil(uint8_t u8Opcode, bool bForward) const;

		template <typename T = uintptr_t>
		const T Get(int pad = 0) const
		{
			return (T)(this->m_uptrAddress + pad);
		}

	private:
		uintptr_t m_uptrAddress = 0;
	};

	/**
	 * @brief Section data holder
	 * 
	 */
	struct Section_t
	{
		const char *m_szName = nullptr;
		uintptr_t m_iStart = 0;
		uintptr_t m_iLen = 0;
	};

	/**
	 * @brief Dll holder and utilities
	 * 
	 */
	struct Dll_t
	{
		Dll_t() = default;
		Dll_t(const char *szName);
		inline ~Dll_t(){};

		template <size_t N>
		const Pointer_t FindPattern(const std::array<int, N> &rgPattern, size_t nMatch = 0u, Hash_t hSection = HASH(".text")) const;

		template <Helveta::CompileTimeString_t Str, bool Conditioned>
		const Pointer_t FindString(uint8_t u8Opcode = 0xFF, int nPad = 4u, Hash_t hSection = HASH(".rdata"));

	private:
		const char *m_szName = nullptr;
		uint8_t *m_prgBytes = nullptr;
		PIMAGE_DOS_HEADER m_pDosHeader = nullptr;
		PIMAGE_NT_HEADERS m_pNtHeaders = nullptr;
		std::unordered_map<Hash_t, Section_t> m_umSections = {};
	};
}

/**
 * @brief Inlined Dll Memory Methods
 * 
 */
template <size_t N>
const Memory::Pointer_t Memory::Dll_t::FindPattern(const std::array<int, N> &rgPattern, size_t nMatch, Hash_t hSection) const
{
	size_t iMatches = 0u;
	const Section_t &range = this->m_umSections.at(hSection);

	for (uintptr_t i = range.m_iStart; i < range.m_iStart + (range.m_iLen - N); ++i)
	{
		bool bFound = true;
		for (size_t j = 0; j < N; ++j)
		{
			if (this->m_prgBytes[i + j] != rgPattern[j] && rgPattern[j] != -1)
			{
				bFound = false;
				break;
			}
		}

		if (bFound)
		{
			if (iMatches != nMatch)
			{
				++iMatches;
				continue;
			}
			else
				return Pointer_t((uintptr_t)(&this->m_prgBytes[i]));
		}
	}

	return Pointer_t(0);
}

template <Helveta::CompileTimeString_t Str, bool Conditioned>
const Memory::Pointer_t Memory::Dll_t::FindString(uint8_t u8Opcode, int nPad, Hash_t hSection)
{
	uintptr_t uptrString = FindPattern(CAST_ARRAY(int, FOLD_LITERALS_INTO_ARRAY(Str.m_szData)), 0, hSection).Get();
	const std::array<int, 4> &rgReversedString = RT_CAST_ARRAY(int, Helveta::detail::ToArray32bit(Helveta::detail::EndiannessSwap32bit(uptrString)));

	Pointer_t xref = FindPattern(rgReversedString);
	if (Conditioned)
	{
		size_t iCount = 0;
		while (*xref.Get<uint8_t *>(nPad) != u8Opcode)
		{
			++iCount;
			xref = FindPattern(rgReversedString, iCount);
		}
	}

	assert(xref.Get());
	return xref;
}