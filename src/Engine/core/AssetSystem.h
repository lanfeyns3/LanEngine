#pragma once

#include <unordered_map>
#include <memory>
#include <utility>
#include "Assets.h"

#include <nlohmann/json.hpp>

#include "WindowSystem.h"
#include "Threading.h"

namespace LANE
{
    class AssetSystem
    {
    public:
        AssetSystem(Threading& Threads,WindowSystem& Windows)
            : threads(Threads), windows(Windows)
        {}

        template<typename T,typename... _Args>
        std::shared_ptr<T> LoadAsset(uint64_t id,nlohmann::json jsonFile, _Args&&... args)
        {
            m_assets[id] = std::make_shared<T>(std::forward<_Args>(args)...);
            m_assets[id]->Load(jsonFile);
            return std::dynamic_pointer_cast<T>(m_assets[id]);
        }

        template<typename T, typename... Args>
        void LoadAssetAsync(uint64_t id, const char* pathBuffer, Args&&... args)
        {
            threads.AddThread(
                [this, id, path = std::string(pathBuffer), args = std::make_tuple(std::forward<Args>(args)...)]() mutable
                {
                    glfwMakeContextCurrent(windows.GetWindow("ManorEngineRendererLoader").first);
                    std::ifstream f(path);
                
                    nlohmann::json jsonFile;
                    f >> jsonFile;
                
                    m_assets[id] = std::apply(
                        [](auto&&... args) {
                            return std::make_shared<T>(
                                std::forward<decltype(args)>(args)...
                            );
                        },
                        std::move(args)
                    );
                
                    m_assets[id]->Load(jsonFile);
                }
            );
            free((void*)pathBuffer);
        }


        template<typename T>
        std::shared_ptr<T> GetAsset(uint64_t id)
        {
            return std::dynamic_pointer_cast<T,Asset>(m_assets[id]);
        }

    private:
        Threading& threads;
        WindowSystem& windows;
        std::unordered_map<uint64_t,std::shared_ptr<Asset>> m_assets;
    };
} // namespace LANE
