#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#undef CreateWindow

#include "core/AssetSystem.h"
#include "core/SceneManager.h"
#include "core/LayerSystem.h"
#include "core/WindowSystem.h"

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

    private:
        AssetSystem& assets;
        SceneManager& scenes;
        LayerSystem& layers;
        WindowSystem& windows;
    };

} // namespace LANE