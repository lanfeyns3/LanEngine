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
#include <nfd.h>

#include "renderer/VBO.h"
#include "renderer/EBO.h"
#include "AssetSystem.h"

#include <iostream>
#include <fstream>

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

        struct Light
        {
            float radius = 10;
            float intensity = .5;
            glm::vec3 color{1.0f};

            void RenderImGui()
            {
                if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::DragFloat("Radius",&radius);
                    ImGui::DragFloat("Intensity",&intensity);
                    ImGui::DragFloat3("Color",glm::value_ptr(color));
                }
            }
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
            Mesh(AssetSystem& assets,std::string Path)
            {
                path = Path;

                std::ifstream f(path);
                nlohmann::json file = nlohmann::json::parse(f);

                const char* newText = (const char*)malloc(path.size() + 1);
                std::memcpy((char*)newText, path.c_str(), path.size() + 1);

                assets.LoadAssetAsync<MeshAsset>(file["UUID"],newText);
                uuid = file["UUID"];
            }

            void RenderImGui()
            {
                if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    std::string format = std::format("Path: {}",path);
                    ImGui::InputText(
                        "##Path",
                        &format,
                        ImGuiInputTextFlags_ReadOnly
                    );
                    ImGui::SameLine();
                    if (ImGui::Button("Open"))
                    {
                        nfdu8char_t *outPath;
                        nfdu8filteritem_t filters[1] = { { "Lane Mesh", "mesh" }};
                        nfdopendialogu8args_t args = {0};
                        args.filterList = filters;
                        args.filterCount = 1;
                        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

                        switch (result)
                        {
                        case NFD_OKAY:
                        {
                            path = outPath;
                            NFD_FreePathU8(outPath);

                            std::ifstream f(path);
                            nlohmann::json file = nlohmann::json::parse(f);

                            uuid = file["UUID"];
                            pathUpdate = true;
                            break;
                        }
                        
                        default:
                            break;
                        }
                    }
                }
            }

            uint64_t uuid;
            std::string path;

            bool pathUpdate = false;
        };
    } // namespace Components
    
} // namespace LANE
