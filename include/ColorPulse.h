#pragma once

#include <rpi-led-matrix/led-matrix.h>

class ColorPulse {
private:
    FrameCanvas* canvas;
    
public:
    ColorPulse(FrameCanvas *c);
    ~ColorPulse();

    void run();
}