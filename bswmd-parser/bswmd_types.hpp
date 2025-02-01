#pragma once

#include <limits>
#include <cstdint>

namespace AutosarGen
{
    typedef uint32_t multiplicity_t;
    static constexpr multiplicity_t infinity() noexcept { return std::numeric_limits<multiplicity_t>::max(); }
}