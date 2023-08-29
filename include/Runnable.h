#pragma once

#include "rpi-rgb-led-matrix/led-matrix.h"
#include "Command.h"


class Runnable {
public:
    rgb_matrix::RGBMatrix* matrix;
    rgb_matrix::FrameCanvas* canvas;

public:
    Runnable(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): matrix(m), canvas(c) {}
    ~Runnable() = default;

    virtual void run() {};
    virtual void setCommand(Command cmd) {};
    virtual void setInitialState() {};
    virtual void bind() {};
};
