#pragma once

class Window {
public:
    Window(int minWidth, int minHeight) : m_minWidth(minWidth), m_minHeight(minHeight) {}
    virtual ~Window() = default;
    virtual void use() = 0;
    virtual void unuse() = 0;
    virtual int windowShouldClose() const = 0;

    int getWidth() const { return m_minWidth; }
    int getHeight() const { return m_minHeight; }
    float getDeltaTime() const { return m_deltaTime; }

protected:
    int m_minWidth, m_minHeight;

    float m_deltaTime = 0.0f, m_prevFrame = 0.0f;
};
