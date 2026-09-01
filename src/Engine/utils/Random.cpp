#include "Random.h"

#include <random>

namespace LANE
{
    namespace
    {
        std::mt19937_64 g_Rng{ std::random_device{}() };
    }

    void SeedRandom(std::uint64_t seed)
    {
        g_Rng.seed(seed);
    }

    std::uint32_t RandomUInt()
    {
        return static_cast<std::uint32_t>(g_Rng());
    }

    std::uint32_t RandomUInt(std::uint32_t min, std::uint32_t max)
    {
        std::uniform_int_distribution<std::uint32_t> dist(min, max);
        return dist(g_Rng);
    }

    std::uint64_t RandomUInt64()
    {
        return g_Rng();
    }

    std::uint64_t RandomUInt64(std::uint64_t min, std::uint64_t max)
    {
        std::uniform_int_distribution<std::uint64_t> dist(min, max);
        return dist(g_Rng);
    }

    int RandomInt(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(g_Rng);
    }

    float RandomFloat()
    {
        return std::uniform_real_distribution<float>(0.0f, 1.0f)(g_Rng);
    }

    float RandomFloat(float min, float max)
    {
        return std::uniform_real_distribution<float>(min, max)(g_Rng);
    }
}