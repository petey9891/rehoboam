#pragma once

#include <rpi-rgb-led-matrix/led-matrix.h>

class PixelApplication {
public:
    rgb_matrix::RGBMatrix* matrix;
    rgb_matrix::FrameCanvas* canvas;

public:
    virtual ~PixelApplication() = default;

    virtual void activate() = 0;
    virtual void run() = 0;
};
