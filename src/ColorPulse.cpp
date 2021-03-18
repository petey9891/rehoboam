#include <cstdint>
#include "ColorPulse.h"

ColorPulse::ColorPulse(FrameCanvas* c): canvas(c) {}

// https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/examples-api-use/demo-main.cc

ColorPulse::run() {
    uint32_t continuum = 0;

    while (true) {
        usleep(5 * 1000);
        continuum += 1;
        continuum %= 3 * 255;

        int r = 0, g = 0, b = 0;

        if (continuum <= 255) {
            int c = continuum;
            b = 255 - c;
            r = c;
        } else if (continuum > 255 && continuum <= 511) {
            int c = continuum - 256;
            r = 255 - c;
            g = c;
        } else {
            int c = continuum - 512;
            g = 255 - c;
            b = c;
        }
        this-=>canvas->Fill(r, g, b);
        off_screen_canvas_ = matrix_->SwapOnVSync(off_screen_canvas_);
    }
};