#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>

#include <functional>

#include "renderer/VBO.h"
#include "renderer/EBO.h"

#include "WindowSystem.h"

namespace LANE
{
    struct Asset
    {
        bool loaded = false;
        
        virtual void Load(nlohmann::json f, std::function<void()> activateOpenglMutex, std::function<void()> deActivateOpenglMutex) = 0;
    };

    struct MeshAsset : public Asset
    {
        void Load(nlohmann::json f,std::function<void()> activateOpenglMutex, std::function<void()> deActivateOpenglMutex);

        uint64_t uuid;
        uint32_t indiceCount;
        
        VBO vbo;
        EBO ebo;
    };

    struct ShaderAsset : public Asset
    {
        ShaderAsset(GLFWwindow* Window)
            : window(Window)
        {}
        void Load(nlohmann::json f,std::function<void()> activateOpenglMutex, std::function<void()> deActivateOpenglMutex);

        unsigned int shaderProgram;

        GLFWwindow* window;
    };
} // namespace LANE
