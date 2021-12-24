#include <Christmas.h>

Christmas::Christmas(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c) {
    printf(">>> <Christmas> Initializing Christmas application\n");
    printf(">>> <Christmas> Initialized Christmas application\n");
    c->SetBrightness(50);
}


void Christmas::setCommand(Command cmd) {
    // do nothing
}

void Christmas::run() {
    usleep(10 * 1000);
    this->continuum += 1;
    this->continuum %= 3 * 255;

    int r = 0, g = 0, b = 0;

    if (this->continuum <= 255) {
        int c = this->continuum;
        g = 255 - c; // starts with green first
        r = c;
    } else if (this->continuum > 255 && this->continuum <= 511) {
        int c = this->continuum - 256;
        r = 255 - c;
        g = c;
    } else {
        this->continuum -= 512;
    }

    this->canvas->Fill(r, g, b);
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
}