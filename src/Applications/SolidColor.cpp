#include <SolidColor.h>

SolidColor::SolidColor(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c), color { 0, 0, 0 } {
    printf(">>> <SolidColor> Initializing SolidColor application\n");
    printf(">>> <SolidColor> Initialized SolidColor application\n");
}

void SolidColor::setCommand(Command cmd) {
    if (cmd.type == StaticColor) {
        if (cmd.data.size() == 3) {
            this->color = { cmd.data[0], cmd.data[1], cmd.data[2] };
            this->canvas->Fill(this->color.r, this->color.g, this->color.b);
        }
    } else if (cmd.type == Brightness) {
        const uint8_t brightness = cmd.data[0];
        printf("brightness: %d\n", brightness);

        this->color.r *= brightness;
        this->color.b *= brightness;
        this->color.g *= brightness;

        printf("r: %d\n", this->color.r);
        printf("g: %d\n", this->color.g);
        printf("b: %d\n", this->color.b);

        this->canvas->Fill(this->color.r, this->color.g, this->color.b);
    }
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
}