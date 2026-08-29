// Assets.cpp
#include "Assets.h"

#include <fstream>
#include <string>
#include <iostream>

namespace LANE
{
    void ShaderAsset::Load(nlohmann::json f)
    {
        glfwMakeContextCurrent(NULL);
        glfwMakeContextCurrent(window);

        unsigned int vertexShader = 0, fragmentShader = 0;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        std::string vSource = f["vertexShaderCode"];
        std::string fSource = f["fragmentShaderCode"];

        const char* vCode = vSource.c_str();
        const char* fCode = fSource.c_str();

        glShaderSource(vertexShader, 1, &vCode, NULL);
        glShaderSource(fragmentShader, 1, &fCode, NULL);

        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader); 

        glfwMakeContextCurrent(NULL);
    }
}