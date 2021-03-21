#pragma once

#include <rpi-led-matrix/led-matrix.h>

class Runnable {
protected:
    rgb_matrix::FrameCanvas* canvas;

public:
    Runnable(rgb_matrix::FrameCanvas* c): canvas(c) {}
    ~Runnable() = default;

    virtual void run() {};
    virtual void bind() {};
};
