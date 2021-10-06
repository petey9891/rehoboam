#include <Clock.h>

Clock::Clock(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c) {
    printf(">>> <Clock> Initializing Clock application\n");
    printf(">>> <Clock> Initialized Clock application\n");
}

void Clock::run() {
    this->canvas->Fill(50, 102, 168);
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
};