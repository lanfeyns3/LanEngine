#pragma once

#include <glad/glad.h>
#include <vector>

namespace LANE
{
    class VBO
    {
    public:
        void Create(const std::vector<float>& vertices, size_t size);
        void Free();
        
        void Load();
        void Unload();
    private:
        GLuint vbo;
    };
} // namespace LANE
