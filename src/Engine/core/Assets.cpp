// Assets.cpp
#include "Assets.h"

#include <fstream>
#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

    void ShaderAsset::Load(nlohmann::json f,std::function<void()> activateOpenglMutex)
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
    
    void MeshAsset::Load(nlohmann::json f,std::function<void()> activateOpenglMutex)
    {
        loaded = false;

        std::string meshSource = f["MeshSource"];
        uuid = f["UUID"];

        Assimp::Importer importer;
        
        // Read the file and apply post-processing steps:
        // - aiProcess_Triangulate: Ensures all shapes are broken down into triangles.
        // - aiProcess_GenSmoothNormals: Generates normals if the model lacks them.
        // - aiProcess_JoinIdenticalVertices: Optimizes vertex count via indexing.
        const aiScene* scene = importer.ReadFile(
            meshSource,
            aiProcess_Triangulate | 
            aiProcess_GenSmoothNormals | 
            aiProcess_JoinIdenticalVertices
        );

        if (!scene) {
            std::cerr << "DEBUG: scene is nullptr!\n";
        } else if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
            std::cerr << "DEBUG: scene is incomplete!\n";
        } else if (!scene->mRootNode) {
            std::cerr << "DEBUG: scene root node is null!\n";
        }

        if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
        {
            std::cerr << "Failed to load mesh with Assimp: " << meshSource 
                      << " | Error: " << importer.GetErrorString() << '\n';
            return;
        }

        // For simplicity, this example extracts the first mesh found in the file 
        // (similar to how single-mesh OBJ files were handled previously).
        if (scene->mNumMeshes == 0)
        {
            std::cerr << "No meshes found in file: " << meshSource << '\n';
            return;
        }

        aiMesh* mesh = scene->mMeshes[0];

        std::vector<float> data;
        std::vector<GLuint> indices;

        // Reserve memory to avoid unnecessary reallocations
        data.reserve(mesh->mNumVertices * 6); // 3 for position, 3 for normal
        indices.reserve(mesh->mNumFaces * 3);

        // Extract vertex data (Positions and Normals)
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            // Positions
            data.push_back(mesh->mVertices[i].x);
            data.push_back(mesh->mVertices[i].y);
            data.push_back(mesh->mVertices[i].z);

            // Normals (Assimp guarantees normals exist if aiProcess_GenSmoothNormals is used)
            if (mesh->HasNormals())
            {
                data.push_back(mesh->mNormals[i].x);
                data.push_back(mesh->mNormals[i].y);
                data.push_back(mesh->mNormals[i].z);
            }
            else
            {
                data.push_back(0.0f);
                data.push_back(0.0f);
                data.push_back(0.0f);
            }
        }

        // Extract index data
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace& face = mesh->mFaces[i];
            
            // Because we used aiProcess_Triangulate, every face should be a triangle (3 indices)
            if (face.mNumIndices != 3)
            {
                std::cerr << "Warning: Non-triangular face encountered, skipping.\n";
                continue;
            }

            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }

        activateOpenglMutex();
        vbo.Create(data, data.size());
        indiceCount = static_cast<GLsizei>(indices.size());
        ebo.Create(indices, indiceCount);

        loaded = true;
        glfwMakeContextCurrent(NULL);
    }
}
