#pragma once

#include <Runnable.h>
#include <rpi-led-matrix/graphics.h>
#include <rpi-led-matrix/pixel-mapper.h>

class Clock: public Runnable {
public:
    Clock(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c);
    virtual ~Clock() = default;

    void run() override;
    void setCommand(Command cmd) override;

private:
    char buffer[256];
    char weekday[256];
    char month[256];
    char date[256];
    char seconds[256];
    char amPm[256];

    struct timespec nextTime;
    struct tm tm;
    
    rgb_matrix::Font font;
    rgb_matrix::Font tinyFont;

    int heightOffset = 3;
    std::string format = "%I:%M:%S%p";
};
