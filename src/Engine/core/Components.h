#pragma once

#include <stdint.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <inttypes.h>
#include <tiny_obj_loader.h>
#include <nlohmann/json.hpp>

#include "renderer/VBO.h"
#include "renderer/EBO.h"

#include <iostream>

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

        struct Camera
        {
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
            glm::vec3 rotation;

            float fov = 90.0f;
            float nearPlane = 0.1f;
            float farPlane  = 100.0f;
        };

        struct Mesh
        {
            Mesh(nlohmann::json path)
            {

                tinyobj::attrib_t attrib;
                std::vector<tinyobj::shape_t> shapes;
                std::vector<tinyobj::material_t> materials;

                std::string warn;
                std::string err;

                std::string meshSource = path["MeshSource"];

                uuid = path["UUID"];

                bool success = tinyobj::LoadObj(
                    &attrib,
                    &shapes,
                    &materials,
                    &warn,
                    &err,
                    meshSource.c_str()
                );

                size_t vertexCount = attrib.vertices.size() / 3;
                vbo.Create(attrib.vertices,vertexCount);

                std::vector<GLuint> indices;

                for (const auto& index : shapes[0].mesh.indices)
                {
                    indices.push_back(static_cast<GLuint>(index.vertex_index));
                }

                indiceCount = indices.size();

                ebo.Create(indices,indiceCount);
            }

            uint64_t uuid;
            uint32_t indiceCount;
            VBO vbo;
            EBO ebo;
        };
    } // namespace Components
    
} // namespace LANE
