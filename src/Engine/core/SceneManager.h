#pragma once

#include <format>
#include <string>
#include <stdint.h>
#include <entt/entt.hpp>
#include <unordered_map>

#include "Components.h"

namespace LANE
{
    class Scene
    {
    public:
        void AddEntity(uint64_t id,std::string name)
        {
            auto entity = registry.create();
            std::string entityName = name;
            if (entityName == "New Object")
            {
                entityName.append(std::format("({})",registry.view<Components::Info>().size()));
            }
            registry.emplace<Components::Info>(entity,id,entityName);
        };

        template<typename T, typename... _Args>
        void AddComponent(uint64_t id,_Args&&... args)
        {
            for (auto& entity : registry.view<Components::Info>())
            {
                auto& info = registry.get<Components::Info>(entity);
                if (info.uuid == id)
                {
                    registry.emplace<T>(entity,std::forward<_Args>(args)...);
                    break;
                }
            }
        }

        template<typename T>
        bool HasComponent(entt::entity entity)
        {
            return registry.all_of<T>(entity);
        }

        template<typename... T>
        auto View()
        {
            return registry.view<T...>();
        }

        template<typename T>
        T& Get(entt::entity entity)
        {
            return registry.get<T>(entity);
        }

    private:
        entt::registry registry;
    };

    class SceneManager
    {
    public:
        void AddScene(uint64_t id);
        void AddEntity(uint64_t sceneID, uint64_t id,std::string name = "New Object");
        entt::entity Get(uint64_t sceneID,uint64_t id);

        template<typename... T>
        auto View(uint64_t sceneID)
        {
            return m_scenes[sceneID].View<T...>();
        }

        uint64_t GetCurrentScene();
        
        template<typename T, typename... _Args>
        void AddComponent(uint64_t sceneID, uint64_t id, _Args&&... args)
        {
            m_scenes[sceneID].AddComponent<T>(id,std::forward<_Args>(args)...);
        }
        template<typename T>
        T& GetComponent(uint64_t sceneID,entt::entity entity)
        {
            return m_scenes[sceneID].Get<T>(entity);
        }

        template<typename T>
        bool HasComponent(uint64_t sceneID,entt::entity id)
        {
            return m_scenes[sceneID].HasComponent<T>(id);
        }
    private:
        uint64_t m_currentScene;
        std::unordered_map<uint64_t, Scene> m_scenes;
    };
} // namespace LANE
