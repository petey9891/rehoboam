#include <Christmas.h>

Christmas::Christmas(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c) {
    printf(">>> <Christmas> Initializing Christmas application\n");
    printf(">>> <Christmas> Initialized Christmas application\n");
    
}

void Christmas::setCommand(Command cmd) {
    if (cmd.type == Brightness) {
        if (cmd.data.size() == 1) {
            const uint8_t data = cmd.data[0];
            if (data >= 1 && data <= 100) {
                // Get the brightness from the data
                this->matrix->SetBrightness(data);
            }
        }
    }
}

void Christmas::run() {
    usleep(6 * 1000);
    this->continuum += 1;
    this->continuum %= 2 * 255;

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