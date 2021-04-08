#pragma once

#include <Runnable.h>
#include <unistd.h>
#include <stdint.h>
#include <Command.h>

class ColorPulse: public Runnable {
public:
    ColorPulse(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c);
    virtual ~ColorPulse() = default;

    void run() override;
    void setCommand(Command cmd) override;

private:
    const float DEVICE_BRIGHTNESS = 0.3f;
    float COLOR_STEP = 0.1f;

    float userBrightness = 1.0f;
    float expectedBrightness;
    
    
    uint32_t continuum = 0;
};