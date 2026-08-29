#pragma once

#include <unordered_map>
#include <memory>
#include <utility>
#include "Assets.h"

#include <nlohmann/json.hpp>

#include "WindowSystem.h"

namespace LANE
{
    class AssetSystem
    {
    public:
        template<typename T,typename... _Args>
        std::shared_ptr<T> LoadAsset(uint64_t id,nlohmann::json jsonFile, _Args&&... args)
        {
            m_assets[id] = std::make_shared<T>(std::forward<_Args>(args)...);
            m_assets[id]->Load(jsonFile);
            return std::dynamic_pointer_cast<T>(m_assets[id]);
        }

        template<typename T>
        std::shared_ptr<T> GetAsset(uint64_t id)
        {
            return std::dynamic_pointer_cast<T,Asset>(m_assets[id]);
        }

    private:
        std::unordered_map<uint64_t,std::shared_ptr<Asset>> m_assets;
    };
} // namespace LANE
