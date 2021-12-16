#pragma once

#include <vector>

#include <Debug.h>

struct VertexBufferElement {
    unsigned int id;
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type) {
        switch (type)
        {
            case GL_FLOAT         : return sizeof(GLfloat);
            case GL_UNSIGNED_INT  : return sizeof(GLuint);
            case GL_UNSIGNED_BYTE : return sizeof(GLbyte);
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout {
    private:
        unsigned int m_Stride;
        std::vector<VertexBufferElement> m_Elements;

    public:
        VertexBufferLayout():
            m_Stride(0) { }

        void addFloat(unsigned int id, unsigned int count)        { push(id, GL_FLOAT, count, GL_FALSE);        }
        void addUnsignedInt(unsigned int id, unsigned int count)  { push(id, GL_UNSIGNED_INT, count, GL_FALSE); }
        void addUnsignedByte(unsigned int id, unsigned int count) { push(id, GL_UNSIGNED_BYTE, count, GL_TRUE); }

        inline const std::vector<VertexBufferElement> getElements() const { return m_Elements; };
        inline unsigned int getStride() const { return m_Stride; };

    private:
        void push(unsigned int id, unsigned int type, unsigned int count, unsigned char normalized) {
            struct VertexBufferElement vbe = {id, type, count, normalized};
            m_Elements.push_back(vbe);

            printf("addFloat id: %d\n", id);
            printf("addFloat count: %d\n", count);
            printf("addFloat size: %d\n", VertexBufferElement::getSizeOfType(type));
            printf("addFloat stride: %d\n", count * VertexBufferElement::getSizeOfType(type));

            m_Stride += count * VertexBufferElement::getSizeOfType(type);
        };
};
