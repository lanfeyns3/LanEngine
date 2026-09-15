#pragma once

#include <unordered_map>
#include <memory>
#include <utility>
#include <iostream>
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
            {
                std::lock_guard<std::mutex> lock(assetsMutex);

                std::cout << id << "\n";

                for (auto asset : m_assets)
                {
                    if (asset.first == id)
                    {
                        free((void*)pathBuffer);
                        return;
                    }
                }

                m_assets[id] = nullptr;
            }

            std::string path(pathBuffer);
            free((void*)pathBuffer);

            threads.AddThread(
                [this, id, path = std::move(path), args = std::make_tuple(std::forward<Args>(args)...)]() mutable
                {
                    glfwMakeContextCurrent(windows.GetWindow("ManorEngineRendererLoader").first);
                
                    std::ifstream f(path);
                    nlohmann::json jsonFile;
                    f >> jsonFile;
                
                    auto newAsset = std::apply(
                        [](auto&&... innerArgs) {
                            return std::make_shared<T>(std::forward<decltype(innerArgs)>(innerArgs)...);
                        },
                        std::move(args)
                    );
                
                    newAsset->Load(jsonFile);

                    {
                        std::lock_guard<std::mutex> lock(assetsMutex);
                        m_assets[id] = std::move(newAsset);
                    }
                }
            );
        }


        template<typename T>
        std::shared_ptr<T> GetAsset(uint64_t id)
        {
            return std::dynamic_pointer_cast<T,Asset>(m_assets[id]);
        }

    private:
        Threading& threads;
        WindowSystem& windows;

        std::mutex assetsMutex;
        std::unordered_map<uint64_t,std::shared_ptr<Asset>> m_assets;
    };
} // namespace LANE
