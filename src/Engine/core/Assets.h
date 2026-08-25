#pragma once

#include <nlohmann/json.hpp>

namespace LANE
{
    struct Asset
    {
        virtual void Load(nlohmann::json f) = 0;
    };

    struct ShaderAsset : public Asset
    {
        ShaderAsset() {}
        void Load(nlohmann::json f);
    };
} // namespace LANE
