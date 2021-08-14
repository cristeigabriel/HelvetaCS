#pragma once

#include <cinttypes>
#include <compare>

struct Color_t
{
	Color_t() = default;
	Color_t(uint8_t u8R, uint8_t u8G, uint8_t u8B, uint8_t u8A = 255);
	inline ~Color_t(){};

	constexpr auto operator<=>(const Color_t &) const = default;

	const Color_t ModifyA(float flMultiplier) const;

	uint8_t m_u8R = 0;
	uint8_t m_u8G = 0;
	uint8_t m_u8B = 0;
	uint8_t m_u8A = 0;
};