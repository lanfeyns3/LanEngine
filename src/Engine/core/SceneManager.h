#pragma once

#include <format>
#include <string>
#include <stdint.h>
#include <entt/entt.hpp>
#include <unordered_map>

#include "Components.h"
#include "AssetSystem.h"
#include "utils/File.h"
#include "utils/Time.h"

#include <nlohmann/json.hpp>

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

        Components::Camera& GetMainCamera()
        {
            return Get<Components::Camera>(m_mainCamera);
        }

        void SetMainCamera(entt::entity camera)
        {
            if (HasComponent<Components::Camera>(camera))
            {
                m_mainCamera = camera;
            }
        }

    private:
        entt::entity m_mainCamera;
        entt::registry registry;
    };

    class SceneManager
    {
    public:
        SceneManager(AssetSystem& assets)
            : m_assets(assets)
        {}
        void AddScene(uint64_t id);
        void AddEntity(uint64_t sceneID, uint64_t id,std::string name = "New Object");
        entt::entity Get(uint64_t sceneID,uint64_t id);

        template<typename... T>
        decltype(auto) View(uint64_t sceneID)
        {
            return m_scenes[sceneID].View<T...>();
        }

        uint64_t GetCurrentScene();
        Components::Camera& GetMainCamera();
        
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

        std::string SaveScene(uint64_t id)
        {
            nlohmann::json json;

            json["UUID"] = id;
            json["Entities"] = nlohmann::json::array();
            for (auto entity : View<Components::Info>(id))
            {
                auto& info = GetComponent<Components::Info>(id,entity);
                
                json["Entities"].push_back({
                    {"Name",info.name},
                    {"UUID",info.uuid},
                    {"Components",nlohmann::json::array()}
                });

                if (HasComponent<Components::Camera>(id,entity))
                {
                    nlohmann::json transformJson;
                    transformJson["Type"] = "Camera";
                    transformJson["Position"] = nlohmann::json::object();
                    transformJson["Rotation"] = nlohmann::json::object();
                    transformJson["Size"] = nlohmann::json::object();

                    auto& transform = GetComponent<Components::Camera>(id, entity);

                    transformJson["Position"]["X"] = transform.position.x;
                    transformJson["Position"]["Y"] = transform.position.y;
                    transformJson["Position"]["Z"] = transform.position.z;

                    transformJson["Rotation"]["X"] = transform.rotation.x;
                    transformJson["Rotation"]["Y"] = transform.rotation.y;
                    transformJson["Rotation"]["Z"] = transform.rotation.z;

                    json["Entities"].back()["Components"].push_back(std::move(transformJson));
                }

                if (HasComponent<Components::Light>(id,entity))
                {
                    nlohmann::json meshJson;
                    meshJson["Type"] = "Light";

                    auto& light = GetComponent<Components::Light>(id, entity);
                    meshJson["Radius"] = light.radius;
                    json["Entities"].back()["Components"].push_back(std::move(meshJson));
                }

                if (HasComponent<Components::Mesh>(id,entity))
                {
                    nlohmann::json meshJson;
                    meshJson["Type"] = "Mesh";

                    auto& mesh = GetComponent<Components::Mesh>(id, entity);
                    meshJson["Source"] = mesh.path;
                    json["Entities"].back()["Components"].push_back(std::move(meshJson));
                }

                if (HasComponent<Components::Renderer>(id,entity))
                {
                    nlohmann::json rendererJson;
                    rendererJson["Type"] = "Renderer";
                    json["Entities"].back()["Components"].push_back(std::move(rendererJson));
                }

                if (HasComponent<Components::Transform>(id,entity))
                {
                    nlohmann::json transformJson;
                    transformJson["Type"] = "Transform";
                    transformJson["Position"] = nlohmann::json::object();
                    transformJson["Rotation"] = nlohmann::json::object();
                    transformJson["Size"] = nlohmann::json::object();

                    auto& transform = GetComponent<Components::Transform>(id, entity);

                    transformJson["Position"]["X"] = transform.position.x;
                    transformJson["Position"]["Y"] = transform.position.y;
                    transformJson["Position"]["Z"] = transform.position.z;

                    transformJson["Rotation"]["X"] = transform.rotation.x;
                    transformJson["Rotation"]["Y"] = transform.rotation.y;
                    transformJson["Rotation"]["Z"] = transform.rotation.z;

                    transformJson["Size"]["X"] = transform.scale.x;
                    transformJson["Size"]["Y"] = transform.scale.y;
                    transformJson["Size"]["Z"] = transform.scale.z;

                    json["Entities"].back()["Components"].push_back(std::move(transformJson));
                }


            }

            return json.dump(4);
        };

        void LoadScenes(nlohmann::json json)
        {
            m_canRender = false;

            m_scenes.clear();

            for (auto& sceneData : json["Scenes"])
            {
                File file(sceneData["Path"]);

                nlohmann::json sceneFile = std::get<nlohmann::json>(file.Read(FileLoadType::Json).value());

                AddScene(sceneFile["UUID"]);
                auto& scene = m_scenes[sceneFile["UUID"]];
                
                auto ret = file.Read(LANE::FileLoadType::Json);
                auto& SceneData = std::get<nlohmann::json>(ret.value());

                for (auto& entityData : SceneData["Entities"])
                {
                    scene.AddEntity(entityData["UUID"],entityData["Name"]);
                    for (auto& componentData : entityData["Components"])
                    {
                        if (componentData["Type"] == "Mesh")
                        {
                            scene.AddComponent<Components::Mesh>(entityData["UUID"],m_assets,componentData["Source"]);
                        }
                        else if (componentData["Type"] == "Light")
                        {
                            scene.AddComponent<Components::Light>(entityData["UUID"]);
                        }
                        else if (componentData["Type"] == "Transform")
                        {
                            scene.AddComponent<Components::Transform>(entityData["UUID"],
                                glm::vec3(componentData["Position"]["X"],componentData["Position"]["Y"],componentData["Position"]["Z"]),
                                glm::vec3(componentData["Rotation"]["X"],componentData["Rotation"]["Y"],componentData["Rotation"]["Z"]),
                                glm::vec3(componentData["Size"]["X"],componentData["Size"]["Y"],componentData["Size"]["Z"])
                            );
                        }
                        else if (componentData["Type"] == "Renderer")
                        {
                            scene.AddComponent<Components::Renderer>(entityData["UUID"]);
                        }
                        else if (componentData["Type"] == "Camera")
                        {
                            scene.AddComponent<Components::Camera>(entityData["UUID"]);
                        }
                    }
                }
            }

            m_currentScene = m_scenes.begin()->first;
            for (auto entity : m_scenes[m_currentScene].View<Components::Info>())
            {
                auto& info = GetComponent<Components::Info>(m_currentScene,entity);

                if (info.name == "Editor Camera")
                {
                    m_scenes[m_currentScene].SetMainCamera(entity);
                    break;
                }
            }

            m_canRender = true;
        }

        bool CanRender()
        {
            return m_canRender;
        }
    private:
        uint64_t m_currentScene;
        std::unordered_map<uint64_t, Scene> m_scenes;

        bool m_canRender = true;
        AssetSystem& m_assets;
    };
} // namespace LANE
