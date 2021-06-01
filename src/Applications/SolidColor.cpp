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
        }
    } else if (cmd.type == Brightness) {
        if (cmd.data.size() == 1) {
            const uint8_t data = cmd.data[0];
            if (data >= 1 && data <= 100) {
                // Get the brightness from the data
                this->expectedBrightness = data / 100.0f;

                // Set the current state, whether it is going up or down
                // change the polarity of the color step depending on change of direction
                if (this->expectedBrightness < this->brightness) {
                    this->state = DECREASING;
                    if (this->COLOR_STEP > 0.0f) {
                        this->COLOR_STEP *= -1.0f;
                    }
                } else if (this->expectedBrightness > this->brightness) {
                    this->state = INCREASING;
                    if (this->COLOR_STEP < 0.0f) {
                        this->COLOR_STEP *= -1.0f;
                    }
                }

                printf("expected brightness: %f\n", this->expectedBrightness);
                printf("brightness: %f\n", this->brightness);
            }
        }
    }
}

void SolidColor::run() {
    if (this->state == DECREASING) {
        // If it is decreasing, update the brightness to go down
        if (this->brightness >= this->expectedBrightness) {
            this->brightness += this->COLOR_STEP;
        } else {
            // If it is done, reset the state
            this->state = NONE;
        }
    } else if (this->state == INCREASING) {
        // If it is increasing, update the brightness to go up
        if (this->brightness < this->expectedBrightness) {
            this->brightness += this->COLOR_STEP;
        } else {
            // If it is done, reset the state
            this->state = NONE;        
        }
    }

    printf("current brightness: %f\n", this->brightness);
    this->canvas->Fill(
        this->color.r * this->brightness,
        this->color.g * this->brightness,
        this->color.b * this->brightness
    );
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
}