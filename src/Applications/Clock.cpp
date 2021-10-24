#include <Clock.h>
#include <time.h>
#include <filesystem>
#include <iostream>


Clock::Clock(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c) {
    printf(">>> <Clock> Initializing Clock application\n");
    
    this->nextTime.tv_sec = time(nullptr);
    this->nextTime.tv_nsec = 0;


    if (!this->font.LoadFont("./resources/fonts/8x13.bdf")) {
        printf(">>> <Clock> Error: Unable to load font\n");
    }
    
    this->matrix->ApplyPixelMapper(rgb_matrix::FindPixelMapper("Rotate", 1, 1, "90"));

    printf(">>> <Clock> Initialized Clock application\n");
}

void Clock::setCommand(Command cmd) {
    // do nothing
};

void Clock::run() {
    this->canvas->Fill(0, 0, 0);
    localtime_r(&nextTime.tv_sec, &tm);

    strftime(buffer, sizeof(buffer), format.c_str(), &tm);
    // this rotates it 90deg each time. currently trying to rotate each panel at time
    // https://github.com/hzeller/rpi-rgb-led-matrix
    // https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/include/pixel-mapper.h
    // https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/include/led-matrix.h
    // https://github.com/hzeller/rpi-rgb-led-matrix/issues/606
    // https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/examples-api-use/clock.cc
    // https://github.com/Tecate/bitmap-fonts


    rgb_matrix::DrawText(this->canvas, this->font, 0, this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 0, this->font.baseline() * 2, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 0, this->font.baseline() * 3, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 0, this->font.baseline() * 4, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 0, this->font.baseline() * 5, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 0, this->font.baseline() * 6, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);

    rgb_matrix::DrawText(this->canvas, this->font, 64, this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 64, this->font.baseline() * 2, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 64, this->font.baseline() * 3, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 64, this->font.baseline() * 4, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 64, this->font.baseline() * 5, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 64, this->font.baseline() * 6, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);

    rgb_matrix::DrawText(this->canvas, this->font, 128, this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 128, this->font.baseline() * 2, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 128, this->font.baseline() * 3, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 128, this->font.baseline() * 4, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 128, this->font.baseline() * 5, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 128, this->font.baseline() * 6, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);

    // rgb_matrix::DrawText(this->canvas, this->font, this->canvas->width()/2, this->canvas->height()/2 + this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);


    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &nextTime, nullptr);
    
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
    nextTime.tv_sec += 1;
};