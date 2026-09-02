#include "EBO.h"

namespace LANE
{
    void EBO::Create(std::vector<GLuint> indices, size_t size)
    {
        glGenBuffers(1,&ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,indices.data(),GL_STATIC_DRAW);

        Unload();
    }

    void EBO::Free()
    {
        Unload();
        glDeleteBuffers(1,&ebo);
    }

    void EBO::Load()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    }
    
    void EBO::Unload()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }
}