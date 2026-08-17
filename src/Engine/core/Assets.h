#pragma once

#include <volk.h>
#include <nlohmann/json.hpp>

namespace LANE
{
    struct Asset
    {
        virtual void Load(nlohmann::json f) = 0;
    };

    struct ShaderAsset : public Asset
    {
        ShaderAsset(VkDevice& Device)
            : device(Device)
        {};

        void Load(nlohmann::json f);

        VkShaderModule vertex;
        VkShaderModule fragment;
        VkDevice& device;
    };
} // namespace LANE
