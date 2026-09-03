#include "SceneManager.h"

namespace LANE
{
    void SceneManager::AddScene(uint64_t id)
    {
        m_scenes[id];
    }
    void SceneManager::AddEntity(uint64_t sceneID, uint64_t id, std::string name)
    {
        m_scenes[sceneID].AddEntity(id,name);
    }
    entt::entity SceneManager::Get(uint64_t sceneID, uint64_t id)
    {
        auto& scene = m_scenes[sceneID];
        for (auto& entity : scene.View<Components::Info>())
        {
            auto& info = scene.Get<Components::Info>(entity);

            if (info.uuid == id)
                return entity;
        }
        return entt::null;
    }
    uint64_t SceneManager::GetCurrentScene()
    {
        return m_currentScene;
    }

    Components::Camera& SceneManager::GetMainCamera()
    {
        return m_scenes[m_currentScene].GetMainCamera();
    }
}