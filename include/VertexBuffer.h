#pragma once

#include <VertexBufferLayout.h>

class VertexBuffer{
    private:
        unsigned int m_RendererID;

    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void bind() const;
        void unbind() const;
        void addLayout(const VertexBufferLayout& layout); 
};
