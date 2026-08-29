#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>

namespace LANE
{
    struct Asset
    {
        virtual void Load(nlohmann::json f) = 0;
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
