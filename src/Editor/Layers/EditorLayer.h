#pragma once

#include "core/Layer.h"
#include <iostream>

#include "core/Application.h"
#include <fstream>

class EditorLayer : public LANE::Layer
{
public:
    EditorLayer(LANE::Application& app)
        : application(app)
    {LANE::SeedRandom(44);}
    
    void Update()
    {
        
    }

    void ImGuiUpdate()
    {
        ImGui::Begin("Inspector");
        auto entities = application.scenes.View<LANE::Components::Info>(application.scenes.GetCurrentScene());

        for (auto& entity : entities)
        {
            auto& info = application.scenes.GetComponent<LANE::Components::Info>(application.scenes.GetCurrentScene(),entity);
            if (selectables.find(info.uuid) == selectables.end())
                selectables[info.uuid] = false;
            if(ImGui::Selectable(info.name.c_str(),&selectables[info.uuid]))
            {
                selectables[selected] = false;
                selected = info.uuid;
                selectedEntity = entity;
            }
        }
        ImGui::End();

        ImGui::Begin("Properties");
        if (selected != 0)
        {
            auto& info = application.scenes.GetComponent<LANE::Components::Info>(application.scenes.GetCurrentScene(),selectedEntity);
            info.RenderImGui();
            if (application.scenes.HasComponent<LANE::Components::Transform>(application.scenes.GetCurrentScene(),selectedEntity))
            {
                application.scenes.GetComponent<LANE::Components::Transform>(application.scenes.GetCurrentScene(),selectedEntity).RenderImGui();
            }
        }
        ImGui::End();
    }

    void OnEvent(LANE::EventType eType, LANE::Event* event)
    {
        if (eType == LANE::EventType::Key)
        {
            LANE::KeyEvent* keyEvent = (LANE::KeyEvent*)(event);

            if (keyEvent->type == LANE::KeyEventType::KeyStroke)
            {
                if (keyEvent->key == GLFW_KEY_ENTER) // add new entity
                {
                    std::cout << "Add new Entity\n";
                    uint64_t id = LANE::RandomUInt64();
                    application.scenes.AddEntity(application.scenes.GetCurrentScene(),id);
                    application.scenes.AddComponent<LANE::Components::Renderer>(application.scenes.GetCurrentScene(),id);

                    std::ifstream f("./bunny.mesh");
                    nlohmann::json file = nlohmann::json::parse(f);

                    application.scenes.AddComponent<LANE::Components::Mesh>(application.scenes.GetCurrentScene(),id,file); // TODO: Split Mesh component and off shore it to the asset manager
                    application.scenes.AddComponent<LANE::Components::Transform>(
                        application.scenes.GetCurrentScene(),
                        id,
                        glm::vec3(0.0f),
                        glm::vec3(0.0f),
                        glm::vec3(1.0f)
                    );
                }
                else if (keyEvent->key == GLFW_KEY_C)
                {
                    std::cout << "Size: " << application.scenes.View<LANE::Components::Info>(application.scenes.GetCurrentScene()).size();
                }
            }
            
        }
    }

private:
    LANE::Application& application;

    uint64_t selected = 0;
    entt::entity selectedEntity;
    
    std::unordered_map<uint64_t,bool> selectables;
};