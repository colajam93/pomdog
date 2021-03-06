// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Math/Vector2.hpp"
#include "Pomdog/Math/Color.hpp"
#include "Pomdog/Math/Radian.hpp"
#include <cstdint>

namespace Pomdog {

class Particle {
public:
    Vector2 Position {0, 0};
    Vector2 Velocity {1, 1};

    Color StartColor = Color::White;
    Color Color = Color::White;
    float ColorVariance = 0;

    Radian<float> Rotation = 0;
    float RotationVariance = 0;

    float StartSize = 1;
    float Size = 1;
    float SizeVariance = 0;

    float TimeToLive = 2;
};

} // namespace Pomdog
