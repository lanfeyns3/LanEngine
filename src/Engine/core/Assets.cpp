// Assets.cpp
#include "Assets.h"

#include <fstream>
#include <string>
#include <iostream>

#include <tiny_obj_loader.h>

namespace LANE
{
    static void PrintOpenGLError(const char* location)
    {
        GLenum error;

        while ((error = glGetError()) != GL_NO_ERROR)
        {
            std::cerr
                << "[OpenGL Error] "
                << location
                << " | Error: 0x"
                << std::hex
                << error
                << std::dec
                << '\n';
        }
    }

    static bool CheckShaderCompile(
        GLuint shader,
        const char* shaderName)
    {
        GLint success = GL_FALSE;

        glGetShaderiv(
            shader,
            GL_COMPILE_STATUS,
            &success
        );

        if (success == GL_TRUE)
        {
            std::cout
                << "[Shader] "
                << shaderName
                << " compiled successfully.\n";

            return true;
        }

        GLint logLength = 0;

        glGetShaderiv(
            shader,
            GL_INFO_LOG_LENGTH,
            &logLength
        );

        std::string log;

        if (logLength > 0)
        {
            log.resize(logLength);

            glGetShaderInfoLog(
                shader,
                logLength,
                nullptr,
                log.data()
            );
        }

        std::cerr
            << "\n========================================\n"
            << "[Shader ERROR] "
            << shaderName
            << " failed to compile.\n"
            << "========================================\n"
            << log
            << "\n========================================\n\n";

        return false;
    }

    static bool CheckProgramLink(
        GLuint program)
    {
        GLint success = GL_FALSE;

        glGetProgramiv(
            program,
            GL_LINK_STATUS,
            &success
        );

        if (success == GL_TRUE)
        {
            std::cout
                << "[Shader Program] "
                << program
                << " linked successfully.\n";

            return true;
        }

        GLint logLength = 0;

        glGetProgramiv(
            program,
            GL_INFO_LOG_LENGTH,
            &logLength
        );

        std::string log;

        if (logLength > 0)
        {
            log.resize(logLength);

            glGetProgramInfoLog(
                program,
                logLength,
                nullptr,
                log.data()
            );
        }

        std::cerr
            << "\n========================================\n"
            << "[PROGRAM LINK ERROR]\n"
            << "Program: "
            << program
            << "\n========================================\n"
            << log
            << "\n========================================\n\n";

        return false;
    }

    void ShaderAsset::Load(nlohmann::json f)
    {
        std::cout
            << "\n========================================\n"
            << "Loading Shader\n"
            << "========================================\n";

        /*
         * ----------------------------------------
         * OPENGL CONTEXT
         * ----------------------------------------
         */

        if (!window)
        {
            std::cerr
                << "[Shader ERROR] GLFW window is null.\n";

            return;
        }

        glfwMakeContextCurrent(nullptr);
        glfwMakeContextCurrent(window);

        std::cout
            << "[Shader] OpenGL context made current.\n";

        PrintOpenGLError(
            "after making context current"
        );

        /*
         * ----------------------------------------
         * SHADER SOURCE
         * ----------------------------------------
         */

        std::string vSource;
        std::string fSource;

        try
        {
            vSource = f["vertexShaderCode"].get<std::string>();
            fSource = f["fragmentShaderCode"].get<std::string>();
        }
        catch (const std::exception& e)
        {
            std::cerr
                << "[Shader ERROR] Failed to read shader "
                << "source from JSON.\n"
                << e.what()
                << '\n';

            glfwMakeContextCurrent(nullptr);
            return;
        }

        std::cout
            << "[Shader] Vertex shader source length: "
            << vSource.size()
            << '\n';

        std::cout
            << "[Shader] Fragment shader source length: "
            << fSource.size()
            << '\n';

        if (vSource.empty())
        {
            std::cerr
                << "[Shader ERROR] Vertex shader source "
                << "is empty.\n";
        }

        if (fSource.empty())
        {
            std::cerr
                << "[Shader ERROR] Fragment shader source "
                << "is empty.\n";
        }

        /*
         * ----------------------------------------
         * CREATE SHADERS
         * ----------------------------------------
         */

        GLuint vertexShader =
            glCreateShader(GL_VERTEX_SHADER);

        GLuint fragmentShader =
            glCreateShader(GL_FRAGMENT_SHADER);

        if (!vertexShader)
        {
            std::cerr
                << "[Shader ERROR] Failed to create "
                << "vertex shader.\n";
        }
        else
        {
            std::cout
                << "[Shader] Vertex shader ID: "
                << vertexShader
                << '\n';
        }

        if (!fragmentShader)
        {
            std::cerr
                << "[Shader ERROR] Failed to create "
                << "fragment shader.\n";
        }
        else
        {
            std::cout
                << "[Shader] Fragment shader ID: "
                << fragmentShader
                << '\n';
        }

        PrintOpenGLError(
            "after glCreateShader"
        );

        /*
         * ----------------------------------------
         * COMPILE VERTEX SHADER
         * ----------------------------------------
         */

        const char* vCode = vSource.c_str();

        glShaderSource(
            vertexShader,
            1,
            &vCode,
            nullptr
        );

        PrintOpenGLError(
            "after vertex glShaderSource"
        );

        std::cout
            << "[Shader] Compiling vertex shader...\n";

        glCompileShader(vertexShader);

        PrintOpenGLError(
            "after vertex glCompileShader"
        );

        bool vertexCompiled =
            CheckShaderCompile(
                vertexShader,
                "Vertex Shader"
            );

        /*
         * ----------------------------------------
         * COMPILE FRAGMENT SHADER
         * ----------------------------------------
         */

        const char* fCode = fSource.c_str();

        glShaderSource(
            fragmentShader,
            1,
            &fCode,
            nullptr
        );

        PrintOpenGLError(
            "after fragment glShaderSource"
        );

        std::cout
            << "[Shader] Compiling fragment shader...\n";

        glCompileShader(fragmentShader);

        PrintOpenGLError(
            "after fragment glCompileShader"
        );

        bool fragmentCompiled =
            CheckShaderCompile(
                fragmentShader,
                "Fragment Shader"
            );

        /*
         * ----------------------------------------
         * STOP IF COMPILATION FAILED
         * ----------------------------------------
         */

        if (!vertexCompiled || !fragmentCompiled)
        {
            std::cerr
                << "[Shader ERROR] Shader compilation "
                << "failed. Program will not be linked.\n";

            /*
             * Print source to make GLSL debugging easier.
             */

            if (!vertexCompiled)
            {
                std::cerr
                    << "\n========== VERTEX SOURCE ==========\n"
                    << vSource
                    << "\n====================================\n";
            }

            if (!fragmentCompiled)
            {
                std::cerr
                    << "\n========= FRAGMENT SOURCE ==========\n"
                    << fSource
                    << "\n====================================\n";
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            glfwMakeContextCurrent(nullptr);

            return;
        }

        /*
         * ----------------------------------------
         * CREATE PROGRAM
         * ----------------------------------------
         */

        shaderProgram =
            glCreateProgram();

        if (!shaderProgram)
        {
            std::cerr
                << "[Shader ERROR] Failed to create "
                << "shader program.\n";

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            glfwMakeContextCurrent(nullptr);

            return;
        }

        std::cout
            << "[Shader] Created program: "
            << shaderProgram
            << '\n';

        /*
         * ----------------------------------------
         * ATTACH
         * ----------------------------------------
         */

        glAttachShader(
            shaderProgram,
            vertexShader
        );

        PrintOpenGLError(
            "after attaching vertex shader"
        );

        glAttachShader(
            shaderProgram,
            fragmentShader
        );

        PrintOpenGLError(
            "after attaching fragment shader"
        );

        /*
         * ----------------------------------------
         * LINK
         * ----------------------------------------
         */

        std::cout
            << "[Shader] Linking program...\n";

        glLinkProgram(
            shaderProgram
        );

        PrintOpenGLError(
            "after glLinkProgram"
        );

        bool linked =
            CheckProgramLink(
                shaderProgram
            );

        /*
         * ----------------------------------------
         * CLEANUP
         * ----------------------------------------
         */

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        std::cout
            << "[Shader] Deleted shader objects.\n";

        if (!linked)
        {
            std::cerr
                << "[Shader ERROR] Program "
                << shaderProgram
                << " failed to link.\n";

            glDeleteProgram(
                shaderProgram
            );

            shaderProgram = 0;

            glfwMakeContextCurrent(nullptr);

            return;
        }

        /*
         * ----------------------------------------
         * SUCCESS
         * ----------------------------------------
         */

        std::cout
            << "\n========================================\n"
            << "Shader loaded successfully\n"
            << "Program ID: "
            << shaderProgram
            << "\n========================================\n\n";

        PrintOpenGLError(
            "before releasing context"
        );

        glfwMakeContextCurrent(nullptr);
    }
    void MeshAsset::Load(nlohmann::json f)
    {
        loaded = false;
        tinyobj::attrib_t attrib;

        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string warn;
        std::string err;

        std::string meshSource = f["MeshSource"];
        uuid = f["UUID"];

        bool success = tinyobj::LoadObj(
            &attrib,
            &shapes,
            &materials,
            &warn,
            &err,
            meshSource.c_str()
        );

        std::vector<float> data;
        std::vector<GLuint> indices;
        
        for (const auto& index : shapes[0].mesh.indices)
        {
            const size_t vi = 3 * index.vertex_index;
        
            data.emplace_back(attrib.vertices.at(vi + 0));
            data.emplace_back(attrib.vertices.at(vi + 1));
            data.emplace_back(attrib.vertices.at(vi + 2));
        
            if (index.normal_index >= 0)
            {
                const size_t ni = 3 * index.normal_index;
            
                data.emplace_back(attrib.normals.at(ni + 0));
                data.emplace_back(attrib.normals.at(ni + 1));
                data.emplace_back(attrib.normals.at(ni + 2));
            }
            else
            {
                // No normal in the OBJ.
                data.emplace_back(0.0f);
                data.emplace_back(0.0f);
                data.emplace_back(0.0f);
            }
        
            indices.push_back(static_cast<GLuint>(indices.size()));
        }
        vbo.Create(data,data.size());
        indiceCount = indices.size();
        ebo.Create(indices,indiceCount);
        loaded = true;
        glfwMakeContextCurrent(NULL);
    }
}
