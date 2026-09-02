#pragma once

#include <glad/glad.h>

namespace LANE
{
    class EBO
    {
    public:
        void Create();
        void Free();
        
        void Load();
        void Unload();
    private:
        GLuint EBO;
    };
} // namespace LANE
