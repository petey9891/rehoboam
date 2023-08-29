#pragma once

class IWindow {
public:
    virtual ~IWindow() = default;
    virtual void use() = 0;
    virtual void unuse() = 0;

    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual float getDeltaTime() const = 0;
};

