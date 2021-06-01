#include <SolidColor.h>

SolidColor::SolidColor(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c), currentColor { 0, 0, 0 } {
    printf(">>> <SolidColor> Initializing SolidColor application\n");
    printf(">>> <SolidColor> Initialized SolidColor application\n");
}

void SolidColor::setCommand(Command cmd) {
    if (cmd.type == StaticColor) {
        if (cmd.data.size() == 3) {
            // this->currentColor = { cmd.data[0], cmd.data[1], cmd.data[2] };
            this->canvas->Fill(cmd.data[0], cmd.data[1], cmd.data[2]);
            this->canvas = this->matrix->SwapOnVSync(this->canvas);
        }
    }
}