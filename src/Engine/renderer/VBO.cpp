#include "VBO.h"

namespace LANE
{
    void VBO::Create(const std::vector<float>& vertices, size_t size)
    {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VBO::Free()
    {
        Unload();
        glDeleteBuffers(1,&vbo);
    }

    void VBO::Load()
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    }
    
    void VBO::Unload()
    {
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
}