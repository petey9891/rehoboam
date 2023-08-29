#pragma once

#include "IWindow.h"
#include <iostream>

template <typename T>
class Window : public IWindow {

public:
    Window(int minWidth, int minHeight) : m_minWidth(minWidth), m_minHeight(minHeight) {}
    virtual ~Window() = default;

    virtual int getWidth() const override { return m_minWidth; }
    virtual int getHeight() const override { return m_minHeight; }
    virtual float getDeltaTime() const override { return m_deltaTime; }

protected:
    T* m_window;

    int m_minWidth, m_minHeight;
    float m_deltaTime = 0.0f, m_prevFrame = 0.0f;
};
