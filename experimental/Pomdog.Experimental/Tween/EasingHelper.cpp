// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#include "Pomdog.Experimental/Tween/EasingHelper.hpp"

namespace Pomdog {
namespace Detail {
namespace Easings {

template <typename T>
T Quadratic(T time)
{
    static_assert(std::is_floating_point<T>::value,
        "You can only use floating-point types");

    // Quadratic easing
    return time * time;
}

template <typename T>
T Cubic(T time)
{
    static_assert(std::is_floating_point<T>::value,
        "You can only use floating-point types");

    // Cubic easing
    return time * time * time;
}

template <typename T>
T Quartic(T time)
{
    static_assert(std::is_floating_point<T>::value,
        "You can only use floating-point types");

    // Quartic easing
    return time * time * time * time;
}

template <typename T>
T Quintic(T time)
{
    static_assert(std::is_floating_point<T>::value,
        "You can only use floating-point types");

    // Quintic easing
    return time * time * time * time * time;
}

template <typename T>
T Sine(T time)
{
    static_assert(std::is_floating_point<T>::value,
        "You can only use floating-point types");

    // Sinusoidal easing
    return 1 - std::cos(time * MathConstants<T>::PiOver2());
}

template <typename T>
T Exponential(T time)
{
    static_assert(std::is_floating_point<T>::value,
        "You can only use floating-point types");

    // Exponential easing
    return std::pow(2, 10 * (time - 1));
}

template <typename T>
T Circle(T time)
{
    static_assert(std::is_floating_point<T>::value,
        "You can only use floating-point types");

    // Circular easing
    return 1 - std::sqrt(1 - time * time);
}

template <typename T>
T Elastic(T time)
{
    static_assert(std::is_floating_point<T>::value,
        "You can only use floating-point types");

    // Elastic easing
    constexpr auto period = T(0.3);
    constexpr auto s = period / 4;
    const auto postFix = std::pow(2, 10 * (time - 1));
    return (time <= 0 || time >= 1) ? time
        : - (postFix * std::sin(((time - 1) - s) * MathConstants<T>::TwoPi() / period));
}

template <typename T>
T Bounce(T time)
{
    static_assert(std::is_floating_point<T>::value,
        "You can only use floating-point types");

    // Bounce easing
    time = 1 - time;

    if (time < 1/2.75) {
        return 1 - (7.5625 * time * time);
    }
    else if (time < 2/2.75) {
        auto t = time - 1.5/2.75;
        return 1 - (7.5625 * t * t + 0.75);
    }
    else if (time < 2.5/2.75) {
        auto t = time - 2.25/2.75;
        return 1 - (7.5625 * t * t + 0.9375);
    }

    auto postFix = time -= 2.625/2.75;
    return 1 - (7.5625 * postFix * time + 0.984375);
}

template <typename T>
T Back(T time)
{
    static_assert(std::is_floating_point<T>::value,
        "You can only use floating-point types");

    // Back easing
    constexpr auto s = T(1.70158);
    return time * time * ((s + 1) * time - s);
}

// NOTE: explicit instantiations
template float Quadratic<float>(float time);
template float Cubic(float time);
template float Quartic(float time);
template float Quintic(float time);
template float Sine(float time);
template float Exponential(float time);
template float Circle(float time);
template float Elastic(float time);
template float Bounce(float time);
template float Back(float time);

} // namespace Easings
} // namespace Detail
} // namespace Pomdog
