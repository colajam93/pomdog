// Copyright (c) 2013-2016 mogemimi. Distributed under the MIT license.

#pragma once

#include "Pomdog/Graphics/InputClassification.hpp"
#include "Pomdog/Graphics/InputElementFormat.hpp"
#include <cstdint>

namespace Pomdog {

struct InputElement {
    std::uint16_t ByteOffset;
    std::uint16_t InputSlot;
    InputElementFormat Format;
    InputClassification InputSlotClass;
    std::uint16_t InstanceStepRate;
};

} // namespace Pomdog
