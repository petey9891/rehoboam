#pragma once

#include <Runnable.h>

class Clock: public Runnable {
public:
    Clock(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c);
    virtual ~Clock() = default;

    void run() override;
    void setCommand(Command cmd) override;
};
