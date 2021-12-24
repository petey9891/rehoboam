#include <Christmas.h>

Christmas::Christmas(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c) {
    printf(">>> <Christmas> Initializing Christmas application\n");
    printf(">>> <Christmas> Initialized Christmas application\n");
}

void Christmas::run() {
    usleep(5 * 1000);
    this->continuum += 1;
    this->continuum %= 3 * 255;

    int r = 0, g = 0, b = 0;

    printf("continuum: %d", this->continuum);
    if (this->continuum <= 255) {
        int c = this->continuum;
        b = 255 - c;
        r = c;
    } else if (this->continuum > 255 && this->continuum <= 511) {
        int c = this->continuum - 256;
        r = 255 - c;
        g = c;
    } else {
        int c = this->continuum - 512;
        g = 255 - c;
        b = c;
    }

    this->canvas->Fill(r, g, b);
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
}