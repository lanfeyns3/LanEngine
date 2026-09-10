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
    {
        LANE::SeedRandom(44);
        editorCameraID = LANE::RandomUInt64();
        application.scenes.AddEntity(application.scenes.GetCurrentScene(),editorCameraID,"Editor Camera");
        application.scenes.AddComponent<LANE::Components::Camera>(application.scenes.GetCurrentScene(),editorCameraID);
    }
    
    void Update(float dt)
    {
        auto scene = application.scenes.GetCurrentScene();
    
        auto& transform = application.scenes.GetMainCamera();
        
        float speed = 1.0f;

        if (selectedEntity != entt::null)
        {
            auto& selectedTransform = application.scenes.GetComponent<LANE::Components::Transform>(application.scenes.GetCurrentScene(),selectedEntity);

            selectedTransform.rotation.y += 25 * dt;
        }
        
        if (moveForward)
            transform.position.z += speed * dt;
        
        if (moveBackward)
            transform.position.z -= speed * dt;
        
        if (moveLeft)
            transform.position.x += speed * dt;
        
        if (moveRight)
            transform.position.x -= speed * dt;
        
        if (moveUp)
            transform.position.y += speed * dt;
        
        if (moveDown)
            transform.position.y -= speed * dt;
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

        ImGui::Begin("Debug");
        LANE::Components::Camera camera = application.scenes.GetMainCamera();
        ImGui::Text("Camera Pos = X: %.2f, Y: %.2f, Z: %.2f",camera.position.x,camera.position.y,camera.position.z);
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
            if (application.scenes.HasComponent<LANE::Components::Light>(application.scenes.GetCurrentScene(),selectedEntity))
            {
                application.scenes.GetComponent<LANE::Components::Light>(application.scenes.GetCurrentScene(),selectedEntity).RenderImGui();
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
                else if (keyEvent->key == GLFW_KEY_LEFT_ALT)
                {
                    std::cout << "Add new Light\n";
                    uint64_t id = LANE::RandomUInt64();
                    application.scenes.AddEntity(application.scenes.GetCurrentScene(),id);
                    application.scenes.AddComponent<LANE::Components::Light>(
                        application.scenes.GetCurrentScene(),
                        id
                    );
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
                else
                {
                    switch (keyEvent->key)
                    {
                        case GLFW_KEY_W:
                            moveForward = true;
                            break;
                    
                        case GLFW_KEY_S:
                            moveBackward = true;
                            break;
                    
                        case GLFW_KEY_A:
                            moveLeft = true;
                            break;
                    
                        case GLFW_KEY_D:
                            moveRight = true;
                            break;
                    
                        case GLFW_KEY_Q:
                            moveUp = true;
                            break;
                    
                        case GLFW_KEY_E:
                            moveDown = true;
                            break;
                    }
                }
            }

            else if (keyEvent->type == LANE::KeyEventType::KeyReleased)
            {
                switch (keyEvent->key)
                {
                    case GLFW_KEY_W:
                        moveForward = false;
                        break;
                
                    case GLFW_KEY_S:
                        moveBackward = false;
                        break;
                
                    case GLFW_KEY_A:
                        moveLeft = false;
                        break;
                
                    case GLFW_KEY_D:
                        moveRight = false;
                        break;
                
                    case GLFW_KEY_Q:
                        moveUp = false;
                        break;
                
                    case GLFW_KEY_E:
                        moveDown = false;
                        break;
                }
            }
            
        }
    }

private:
    LANE::Application& application;

    uint64_t selected = 0;
    entt::entity selectedEntity = entt::null;
    uint64_t editorCameraID;

    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
    
    std::unordered_map<uint64_t,bool> selectables;
};