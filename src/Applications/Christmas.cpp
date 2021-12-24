#include <Christmas.h>

Christmas::Christmas(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c) {
    printf(">>> <Christmas> Initializing Christmas application\n");
    printf(">>> <Christmas> Initialized Christmas application\n");
}


void Christmas::setCommand(Command cmd) {
    // do nothing
}

void Christmas::run() {
    usleep(5 * 1000);
    this->continuum += 1;
    this->continuum %= 3 * 255;

    int r = 0, g = 0, b = 0;

    printf("BEGINNING r: %d, g: %d, b: %d, continuum: %d\n", r, g, b, this->continuum);
    if (this->continuum <= 255) {
        int c = this->continuum;
        printf("in FIRST section of continuum -- c: %d\n", c);
        b = 255 - c;
        r = c;
    } else if (this->continuum > 255 && this->continuum <= 511) {
        int c = this->continuum - 256;
        printf("in SECOND section of continuum -- c: %d\n", c);
        r = 255 - c;
        g = c;
    } else {
        int c = this->continuum - 512;
        printf("in THIRD section of continuum -- c: %d\n", c);
        g = 255 - c;
        b = c;
    }
    printf("END r: %d, g: %d, b: %d, continuum: %d\n", r, g, b, this->continuum);

    this->canvas->Fill(r, g, b);
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
}