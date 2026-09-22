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

        std::string Serialize(uint64_t id)
        {
            nlohmann::json json;

            json["UUID"] = id;
            json["Entities"] = nlohmann::json::array();
            for (auto entity : View<Components::Info>())
            {
                auto& info = Get<Components::Info>(entity);
                
                json["Entities"].push_back({
                    {"Name", info.name},
                    {"UUID", info.uuid},
                    {"Components", nlohmann::json::array()}
                });

                if (HasComponent<Components::Camera>(entity))
                {
                    auto& camera = Get<Components::Camera>(entity);
                    json["Entities"].back()["Components"].push_back(camera.Serialize());
                }

                if (HasComponent<Components::Light>(entity))
                {
                    auto& light = Get<Components::Light>(entity);
                    json["Entities"].back()["Components"].push_back(light.Serialize());
                }

                if (HasComponent<Components::Mesh>(entity))
                {
                    auto& mesh = Get<Components::Mesh>(entity);
                    json["Entities"].back()["Components"].push_back(mesh.Serialize());
                }

                if (HasComponent<Components::Renderer>(entity))
                {
                    nlohmann::json rendererJson;
                    rendererJson["Type"] = "Renderer";
                    json["Entities"].back()["Components"].push_back(std::move(rendererJson));
                }

                if (HasComponent<Components::Transform>(entity))
                {
                    auto& transform = Get<Components::Transform>(entity);
                    json["Entities"].back()["Components"].push_back(transform.Serialize());
                }
            }

            return json.dump(4);
        }

        std::string GetPath() {return path;}
        void SetPath(std::string Path) {path = std::move(Path);}
        
        void Clear()
        {
            registry.clear();
        }

    private:
        entt::entity m_mainCamera;
        entt::registry registry;

        std::string path = "";
    };

    class SceneManager
    {
    public:
        SceneManager(AssetSystem& assets)
            : m_assets(assets)
        {}
        void AddScene(uint64_t id);
        void AddEntity(uint64_t sceneID, uint64_t id, std::string name = "New Object");
        entt::entity Get(uint64_t sceneID, uint64_t id);

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
            m_scenes[sceneID].AddComponent<T>(id, std::forward<_Args>(args)...);
        }
        template<typename T>
        T& GetComponent(uint64_t sceneID, entt::entity entity)
        {
            return m_scenes[sceneID].Get<T>(entity);
        }

        template<typename T>
        bool HasComponent(uint64_t sceneID, entt::entity id)
        {
            return m_scenes[sceneID].HasComponent<T>(id);
        }

        std::string SaveScene(uint64_t id)
        {
            std::string serializedData = m_scenes[id].Serialize(id);
            return serializedData;
        };

        std::string SnapshotCurrentScene()
        {
            return SaveScene(m_currentScene);
        }

        void LoadCurrentSnapshot(std::string snapshot)
        {
            m_canRender = false;
            nlohmann::json j = nlohmann::json::parse(snapshot);
            m_scenes[m_currentScene].Clear();
            LoadScene(j,m_scenes[m_currentScene]);

            for (auto entity : m_scenes[m_currentScene].View<Components::Info>())
            {
                auto& info = GetComponent<Components::Info>(m_currentScene, entity);

                if (info.name == "Editor Camera")
                {
                    m_scenes[m_currentScene].SetMainCamera(entity);
                    break;
                }
            }
            m_canRender = true;
        }

        void LoadScene(nlohmann::json& SceneData, Scene& scene)
        {
            for (auto& entityData : SceneData["Entities"])
            {
                scene.AddEntity(entityData["UUID"], entityData["Name"]);
                for (auto& componentData : entityData["Components"])
                {
                    if (componentData["Type"] == "Mesh")
                    {
                        scene.AddComponent<Components::Mesh>(entityData["UUID"], m_assets, componentData["Source"]);
                    }
                    else if (componentData["Type"] == "Light")
                    {
                        scene.AddComponent<Components::Light>(entityData["UUID"]);
                    }
                    else if (componentData["Type"] == "Transform")
                    {
                        scene.AddComponent<Components::Transform>(entityData["UUID"],
                            glm::vec3(componentData["Position"]["X"], componentData["Position"]["Y"], componentData["Position"]["Z"]),
                            glm::vec3(componentData["Rotation"]["X"], componentData["Rotation"]["Y"], componentData["Rotation"]["Z"]),
                            glm::vec3(componentData["Size"]["X"], componentData["Size"]["Y"], componentData["Size"]["Z"])
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

                LoadScene(SceneData,scene);
            }

            m_currentScene = m_scenes.begin()->first;
            for (auto entity : m_scenes[m_currentScene].View<Components::Info>())
            {
                auto& info = GetComponent<Components::Info>(m_currentScene, entity);

                if (info.name == "Editor Camera")
                {
                    m_scenes[m_currentScene].SetMainCamera(entity);
                    break;
                }
            }

            m_canRender = true;
        }

        std::string SaveScenes()
        {
            nlohmann::json j;

            j["Scenes"] = nlohmann::json::array();
            
            for (auto& scene : m_scenes)
            {
                if (scene.second.GetPath() == "")
                {
                    File file("");

                    std::vector<nfdu8filteritem_t> filters = {{"Lan Scene","lanscn"}};

                    std::string path = file.PromptFileDialog(LANE::DialogType::Save,filters);
                    file.Write(SaveScene(scene.first));
                    scene.second.SetPath(path);
                }

                nlohmann::json sceneBlock;
                sceneBlock["Path"] = scene.second.GetPath();

                j["Scenes"].push_back(std::move(sceneBlock));
            }

            return j.dump(4);
        }

        bool CanRender()
        {
            return m_canRender;
        }
    private:
        uint64_t m_currentScene = 0;
        std::unordered_map<uint64_t, Scene> m_scenes;

        bool m_canRender = true;
        AssetSystem& m_assets;
    };
} // namespace LANE