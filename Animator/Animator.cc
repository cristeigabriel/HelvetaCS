#include "Animator.hh"

#include <algorithm>

void Animator_t::Set(bool increment, const Animation_t& in, const Animation_t& out) {
    this->m_flAnimationState += increment ? in.m_flSpeed * 0.001f : -out.m_flSpeed * 0.001f;
    this->m_flAnimationState  = std::clamp(this->m_flAnimationState, 0.f, 1.f);
    this->m_flAnimationFactor = increment ? in.m_EaserFn(this->m_flAnimationState) : out.m_EaserFn(this->m_flAnimationState);
    this->m_flAnimationFactor = std::clamp(this->m_flAnimationFactor, 0.f, 1.f);
}

float Animator_t::Get() const {
    return this->m_flAnimationFactor;
}