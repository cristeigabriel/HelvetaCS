#include "Color.hh"
#include <utility>

const Color_t Color_t::ModifyA(float flMultiplier) const
{
	auto _this = *this;
	_this.m_u8A *= std::min(flMultiplier, 1.F);
	return _this;
}