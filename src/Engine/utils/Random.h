#pragma once

#include <cstdint>

namespace LANE
{
    void SeedRandom(std::uint64_t seed);

    std::uint32_t RandomUInt();
    std::uint32_t RandomUInt(std::uint32_t min, std::uint32_t max);

    std::uint64_t RandomUInt64();
    std::uint64_t RandomUInt64(std::uint64_t min, std::uint64_t max);

    int RandomInt(int min, int max);

    float RandomFloat();
    float RandomFloat(float min, float max);
}