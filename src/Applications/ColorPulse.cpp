#include <ColorPulse.h>

ColorPulse::ColorPulse(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c) {
    printf(">>> <ColorPulse> Initializing ColorPulse application\n");
    printf(">>> <ColorPulse> Initialized ColorPulse application\n");
}

void ColorPulse::setCommand(Command cmd) {
    if (cmd.type == Brightness) {
        if (cmd.data.size() > 0) {
            this->expectedBrightness = cmd.data[0] / 100.0f;
        }
    }
}

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

    if (this->expectedBrightness < this->userBrightness || this->expectedBrightness > this->userBrightness) {
        this->COLOR_STEP *= -1.0f;

    if (this->expectedBrightness != this->userBrightness)
        this->userBrightness += this->COLOR_STEP;

    this->canvas->Fill(
        r * this->DEVICE_BRIGHTNESS * this->userBrightness,
        g * this->DEVICE_BRIGHTNESS * this->userBrightness,
        b * this->DEVICE_BRIGHTNESS * this->userBrightness
    );
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
};
