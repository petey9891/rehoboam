#include <SolidColor.h>

SolidColor::SolidColor(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c), color { 0, 0, 0 } {
    printf(">>> <SolidColor> Initializing SolidColor application\n");
    printf(">>> <SolidColor> Initialized SolidColor application\n");
}

void SolidColor::setCommand(Command cmd) {
    if (cmd.type == StaticColor) {
        if (cmd.data.size() == 3) {
            const float r = cmd.data[0];
            const float g = cmd.data[1];
            const float b = cmd.data[2];

            this->color = { r, g, b };
            this->canvas->Fill(this->color.r, this->color.g, this->color.b);
        }
    } else if (cmd.type == Brightness) {
        const float brightness = cmd.data[0] / 10.0f;

        this->canvas->Fill(
            this->color.r *= brightness,
            this->color.b *= brightness,
            this->color.g *= brightness
        );
    }
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
}