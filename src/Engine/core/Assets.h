#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>

#include "renderer/VBO.h"
#include "renderer/EBO.h"

namespace LANE
{
    struct Asset
    {
        virtual void Load(nlohmann::json f) = 0;
    };

    struct MeshAsset : public Asset
    {
        void Load(nlohmann::json f);

        uint64_t uuid;
        uint32_t indiceCount;
        bool loaded = false;
        
        VBO vbo;
        EBO ebo;
    };

    struct ShaderAsset : public Asset
    {
        ShaderAsset(GLFWwindow* Window)
            : window(Window)
        {}
        void Load(nlohmann::json f);

        unsigned int shaderProgram;

        GLFWwindow* window;
    };
} // namespace LANE
