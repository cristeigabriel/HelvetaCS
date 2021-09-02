#pragma once

#include <cinttypes>
#include <compare>

struct Color_t {
	constexpr Color_t() = default;
	constexpr Color_t(uint8_t u8R, uint8_t u8G, uint8_t u8B, uint8_t u8A = 255) {
		this->m_u8R = u8R;
		this->m_u8G = u8G;
		this->m_u8B = u8B;
		this->m_u8A = u8A;
	}
	constexpr inline ~Color_t() {};

	constexpr auto operator<=>(const Color_t&) const = default;

	const Color_t ModifyA(float flMultiplier) const;

	uint8_t m_u8R = 0;
	uint8_t m_u8G = 0;
	uint8_t m_u8B = 0;
	uint8_t m_u8A = 0;
};