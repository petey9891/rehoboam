#include <Clock.h>
#include <time.h>
#include <filesystem>
#include <iostream>


Clock::Clock(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c) {
    printf(">>> <Clock> Initializing Clock application\n");
    
    this->nextTime.tv_sec = time(nullptr);
    this->nextTime.tv_nsec = 0;

    if (!this->tinyFont.LoadFont("./resources/fonts/6x9.bdf")) {
        printf(">>> <Clock> Error: Unable to load font\n");
    }

    if (!this->font.LoadFont("./resources/fonts/9x18B.bdf")) {
        printf(">>> <Clock> Error: Unable to load font\n");
    }

    printf(">>> <Clock> Initialized Clock application\n");
}

void Clock::setCommand(Command cmd) {
    // do nothing
};

void Clock::run() {
    this->canvas->Fill(0, 0, 0);
    localtime_r(&nextTime.tv_sec, &tm);

    strftime(buffer, sizeof(buffer), format.c_str(), &tm);
    strftime(weekday, sizeof(weekday), "%A,", &tm);
    strftime(month, sizeof(month), "%b %d", &tm);
    strftime(seconds, sizeof(seconds), ":%S", &tm);
    strftime(amPm, sizeof(amPm), "%p", &tm);
    
    // rgb_matrix::DrawText(this->canvas, this->font, 0, this->heightOffset + this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, weekday, 0);
    rgb_matrix::DrawText(this->canvas, this->font, 0, this->heightOffset + this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, month, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 0, this->heightOffset + this->font.baseline() * 3, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->tinyFont, 0, this->heightOffset + this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, seconds, 0);
    // rgb_matrix::DrawText(this->canvas, this->tinyFont, 0, this->heightOffset + this->font.baseline() * 2, rgb_matrix::Color(255, 255, 255), nullptr, amPm, 0);

    // rgb_matrix::DrawText(this->canvas, this->font, 64, this->heightOffset + this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 64, this->heightOffset + this->font.baseline() * 2, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 64, this->heightOffset + this->font.baseline() * 3, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 64, this->heightOffset + this->font.baseline() * 4, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 64, this->heightOffset + this->font.baseline() * 5, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);


    // rgb_matrix::DrawText(this->canvas, this->font, 128, this->heightOffset + this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 128, this->heightOffset + this->font.baseline() * 2, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 128, this->heightOffset + this->font.baseline() * 3, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 128, this->heightOffset + this->font.baseline() * 4, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 128, this->heightOffset + this->font.baseline() * 5, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);


    // rgb_matrix::DrawText(this->canvas, this->font, 24, this->heightOffset + this->font.baseline() * 2, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 0,  this->heightOffset + this->font.baseline() * 3, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 24, this->heightOffset + this->font.baseline() * 4, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 0,  this->heightOffset + this->font.baseline() * 5, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // // rgb_matrix::DrawText(this->canvas, this->font, 25, this->heightOffset + this->font.baseline() * 6, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);

    // rgb_matrix::DrawText(this->canvas, this->font, 64, this->heightOffset + this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 88, this->heightOffset + this->font.baseline() * 2, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 64, this->heightOffset + this->font.baseline() * 3, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 88, this->heightOffset + this->font.baseline() * 4, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 64, this->heightOffset + this->font.baseline() * 5, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // // rgb_matrix::DrawText(this->canvas, this->font, 89, this->heightOffset + this->font.baseline() * 6, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);


    // rgb_matrix::DrawText(this->canvas, this->font, 128, this->heightOffset + this->font.baseline(), rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 152, this->heightOffset + this->font.baseline() * 2, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 128, this->heightOffset + this->font.baseline() * 3, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 152, this->heightOffset + this->font.baseline() * 4, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 128, this->heightOffset + this->font.baseline() * 5, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);
    // rgb_matrix::DrawText(this->canvas, this->font, 153, this->heightOffset + this->font.baseline() * 6, rgb_matrix::Color(255, 255, 255), nullptr, buffer, 0);

    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &nextTime, nullptr);
    
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
    nextTime.tv_sec += 1;
};