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
    
    if (rgb_matrix::FindPixelMapper("Rotate", 0, 0, "90") != nullptr) {
        printf("Found rotate mapper");
    } else {
        printf("Did not find rotate mapper");
    };


    printf(">>> <Clock> Initialized Clock application\n");
}

void Clock::setCommand(Command cmd) {
    // do nothing
};

void Clock::run() {
    this->canvas->Fill(0, 0, 0);
    localtime_r(&nextTime.tv_sec, &tm);

    strftime(buffer, sizeof(buffer), format.c_str(), &tm);

    rgb_matrix::DrawText(this->canvas, this->font, 0, 0 + this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 64, 0 + this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 128, 0 + this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);


    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &nextTime, nullptr);
    
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
    nextTime.tv_sec += 1;
};