#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#undef CreateWindow

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/AssetSystem.h"
#include "core/SceneManager.h"
#include "core/LayerSystem.h"
#include "core/WindowSystem.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

namespace LANE
{
    class Renderer
    {
    public:
        Renderer(
            AssetSystem& Assets,
            SceneManager& Scenes,
            LayerSystem& Layers,
            WindowSystem& Windows
        );

    public:
        void RenderScene(GLFWwindow* window);
        void CreateShader(std::string path);
        void RegisterWindowImgui(GLFWwindow * window);

    private:
        AssetSystem& assets;
        SceneManager& scenes;
        LayerSystem& layers;
        WindowSystem& windows;
    private:
        std::unordered_map<GLFWwindow *,ImGuiContext*> m_contexts;
        std::unordered_map<GLFWwindow*,std::unordered_map<uint64_t,VAO>> m_vaos;

        bool tempAdd = false;
    };

} // namespace LANE