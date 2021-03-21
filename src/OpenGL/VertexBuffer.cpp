#include <VertexBuffer.h>
#include <Debug.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GLCall(glGenBuffers(1, &this->m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));

    printf(">>> initialized vertex buffer with id: %d\n", this->m_RendererID);
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &this->m_RendererID));
}

void VertexBuffer::bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->m_RendererID));
}

void VertexBuffer::unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::addLayout(const VertexBufferLayout& layout) {
    this->bind();
    
    const unsigned int stride = layout.getStride(); 
    const std::vector<VertexBufferElement> elements = layout.getElements();
    
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const VertexBufferElement element = elements[i];

        GLCall(glEnableVertexAttribArray(element.id));
        GLCall(glVertexAttribPointer(element.id, element.count, element.type, element.normalized, stride, INT2VOIDP(offset)));

        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}