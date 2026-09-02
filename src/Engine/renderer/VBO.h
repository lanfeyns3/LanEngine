#pragma once

#include <glad/glad.h>

namespace LANE
{
    class VBO
    {
    public:
        void Create(float vertices[], size_t size);
        void Free();
        
        void Load();
        void Unload();
    private:
        GLuint vbo;
    };
} // namespace LANE
