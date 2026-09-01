#pragma once

#include <stdint.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <inttypes.h>

namespace LANE
{
    namespace Components
    {
        struct Info
        {
            uint64_t uuid;
            std::string name;

            void RenderImGui()
            {
                if (ImGui::CollapsingHeader("Info", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::InputText("Name", &name);
                
                    if (name.empty())
                        name = "No Name";

                    ImGui::Text("UUID: %" PRIu64, uuid);
                }
            }
        };

        struct Renderer // TODO: better name
        {

        };

        struct Transform
        {
            glm::vec3 position;
            glm::vec3 rotation;
            glm::vec3 scale;

            Transform(glm::vec3 pos,glm::vec3 rot, glm::vec3 s)
                : position(pos), rotation(rot), scale(s)
            {}

            void RenderImGui()
            {
                if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::DragFloat3("Position",glm::value_ptr(position));
                    ImGui::DragFloat3("Rotation",glm::value_ptr(rotation));
                    ImGui::DragFloat3("Scale",glm::value_ptr(scale));
                }
            }
        };
    } // namespace Components
    
} // namespace LANE
