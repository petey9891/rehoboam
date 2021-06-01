#pragma once

#include <Runnable.h>

class SolidColor: public Runnable {
public:
    SolidColor(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c);
    virtual ~SolidColor() = default;

    void setCommand(Command cmd) override;

private:

    struct RGBColor {
        float r;
        float g;
        float b;
    };

    RGBColor color;
};