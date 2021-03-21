#pragma once

#include <rpi-led-matrix/led-matrix.h>

class ColorPulse {
private:
    RGBMatrix* const matrix;
    FrameCanvas* canvas

public:
    ColorPulse(RGBMatrix *m);
    ~ColorPulse();

    void run();
}