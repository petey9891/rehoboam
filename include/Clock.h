#pragma once

#include <Runnable.h>
#include <rpi-led-matrix/graphics.h>
#include <rpi-led-matrix/pixel-mapper.h>
#include <RotateSinglePanelPixelMapper.h>

class Clock: public Runnable {
public:
    Clock(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c);
    virtual ~Clock() = default;

    void run() override;
    void setCommand(Command cmd) override;

private:
    char buffer[256];
    struct timespec nextTime;
    struct tm tm;
    rgb_matrix::Font font;
    std::string format = "%H:%M";
};
