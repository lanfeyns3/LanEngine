#pragma once

#include "core/Layer.h"
#include <iostream>

#include "core/Application.h"

class EditorLayer : public LANE::Layer
{
public:
    EditorLayer(LANE::Application& app)
        : application(app)
    {}
    void Update()
    {
        
    }

    void ImGuiUpdate()
    {
        ImGui::Begin("Inspector");
        ImGui::Text("Hello, World!");
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
                    application.scenes.AddEntity(application.scenes.GetCurrentScene(),45);
                    application.scenes.AddComponent<LANE::Components::Renderer>(application.scenes.GetCurrentScene(),45);
                    application.scenes.AddComponent<LANE::Components::Transform>(application.scenes.GetCurrentScene(),45);
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
};