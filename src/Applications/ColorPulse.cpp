#include <cstdint>
#include <ColorPulse.h>

ColorPulse::ColorPulse(rgb_matrix::FrameCanvas *c): Runnable(c) {
    printf(">>> <ColorPulse> Initializing ColorPulse application\n");
    printf(">>> <ColorPulse> Initialized ColorPulse application\n");
}
// https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/examples-api-use/demo-main.cc

void ColorPulse::run() {
    usleep(5 * 1000);
    this->continuum += 1;
    this->continuum %= 3 * 255;

    int r = 0, g = 0, b = 0;

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
};