#pragma once

#include <Runnable.h>
#include <stdint.h>

class ColorPulse: public Runnable {
private:
    uint32_t continuum = 0;
public:
    ColorPulse(rgb_matrix::FrameCanvas *c);
    ~ColorPulse() = default;

    void run() override;
};