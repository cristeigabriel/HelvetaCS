#include "Color.hh"
#include <utility>

Color_t::Color_t(uint8_t u8R, uint8_t u8G, uint8_t u8B, uint8_t u8A)
{
	this->m_u8R = u8R;
	this->m_u8G = u8G;
	this->m_u8B = u8B;
	this->m_u8A = u8A;
}

const Color_t Color_t::ModifyA(float flMultiplier) const
{
	auto _this = *this;
	_this.m_u8A *= std::min(flMultiplier, 1.F);
	return _this;
}