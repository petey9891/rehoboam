#pragma once

#include <Runnable.h>
#include <unistd.h>
#include <stdint.h>

class ColorPulse: public Runnable {
private:
    uint32_t continuum = 0;
public:
    ColorPulse(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c);
    virtual ~ColorPulse() = default;

    void run() override;
};