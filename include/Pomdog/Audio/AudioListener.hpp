// Copyright (c) 2013-2015 mogemimi. Distributed under the MIT license.

#ifndef POMDOG_AUDIOLISTENER_C5B766B1_HPP
#define POMDOG_AUDIOLISTENER_C5B766B1_HPP

#include "Pomdog/Math/Vector3.hpp"

namespace Pomdog {

class AudioListener {
public:
    Vector3 Forward;
    Vector3 Position;
    Vector3 Up;
    Vector3 Velocity;
};

} // namespace Pomdog

#endif // POMDOG_AUDIOLISTENER_C5B766B1_HPP
