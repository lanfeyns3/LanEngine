#pragma once

#include <glad/glad.h>
#include <vector>

namespace LANE
{
    class EBO
    {
    public:
        void Create(const std::vector<GLuint>& indices, size_t size);
        void Free();
        
        void Load();
        void Unload();
    private:
        GLuint ebo;
    };
} // namespace LANE
