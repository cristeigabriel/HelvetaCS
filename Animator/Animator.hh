#pragma once

#include <functional>

struct Animation_t {
    float m_flSpeed;
    std::function<float(float)> m_EaserFn;
};

struct Animator_t {
    Animator_t() = default;
    inline ~Animator_t() {};

    void Set(bool bIncrement, const Animation_t& in, const Animation_t& out);
    float Get() const;

  private:
    float m_flAnimationFactor;
    float m_flAnimationState;
};

namespace Easing {
static float OutQuart(float flDelta) {
    flDelta = (--flDelta) * flDelta;
    return (float)(1 - flDelta * flDelta);
}
}  // namespace Easing