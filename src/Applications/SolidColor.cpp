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
        const uint8_t brightness = cmd.data[0] / 100.0f;
        
        printf("brightness: %f\n", brightness);

        this->color.r *= brightness;
        this->color.b *= brightness;
        this->color.g *= brightness;

        printf("r: %f\n", this->color.r);
        printf("g: %f\n", this->color.g);
        printf("b: %f\n", this->color.b);

        this->canvas->Fill(this->color.r, this->color.g, this->color.b);
    }
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
}