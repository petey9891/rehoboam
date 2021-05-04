#include <ColorPulse.h>

ColorPulse::ColorPulse(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c): Runnable(m, c) {
    printf(">>> <ColorPulse> Initializing ColorPulse application\n");
    printf(">>> <ColorPulse> Initialized ColorPulse application\n");
}

void ColorPulse::setCommand(Command cmd) {
    if (cmd.type == Brightness) {
        if (cmd.data.size() > 0) {
            const uint8_t data = cmd.data[0];
            if (data >= 1 && data <= 100) {
                // Get the brightness from the data
                this->expectedBrightness = data / 100.0f;

                // Set the current state, whether it is going up or down
                // change the polarity of the color step depending on change of direction
                if (this->expectedBrightness < this->userBrightness) {
                    this->currentState = DECREASING;
                    if (this->COLOR_STEP > 0.0f) {
                        this->COLOR_STEP *= -1.0f;
                    }
                } else if (this->expectedBrightness > this->userBrightness) {
                    this->currentState = INCREASING;
                    if (this->COLOR_STEP < 0.0f) {
                        this->COLOR_STEP *= -1.0f;
                    }
                }
            }
        }
    }
}

void ColorPulse::run() {
    usleep(5 * 1000);
    this->continuum += 1;
    this->continuum %= 3 * 255;
    // this->continuum %= 3 * 178;

    int r = 0, g = 0, b = 0;


    // performance: 198.6Hz max: 6412usec
    // if (this->continuum <= 178) {
    //     int c = this->continuum;
    //     b = 178 - c;
    //     r = c;
    // } else if (this->continuum > 178 && this->continuum <= 356) {
    //     int c = this->continuum - 179;
    //     r = 178 - c;
    //     g = c;
    // } else {
    //     int c = this->continuum - 356;
    //     g = 178 - c;
    //     b = c;
    // }

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

    // if (this->currentState == DECREASING) {
    //     // If it is decreasing, update the brightness to go down
    //     if (this->userBrightness >= this->expectedBrightness) {
    //         this->userBrightness += this->COLOR_STEP;
    //     } else {
    //         // If it is done, reset the state
    //         this->currentState = NONE;
    //     }
    // } else if (this->currentState == INCREASING) {
    //     // If it is increasing, update the brightness to go up
    //     if (this->userBrightness < this->expectedBrightness) {
    //         this->userBrightness += this->COLOR_STEP;
    //     } else {
    //         // If it is done, reset the state
    //         this->currentState = NONE;        
    //     }
    // }

    this->canvas->Fill(
        r-76,
        g-76,
        b-76
    );

    // this->canvas->Fill(
    //     r * this->DEVICE_BRIGHTNESS * this->userBrightness,
    //     g * this->DEVICE_BRIGHTNESS * this->userBrightness,
    //     b * this->DEVICE_BRIGHTNESS * this->userBrightness
    // );

    
    this->canvas = this->matrix->SwapOnVSync(this->canvas);
};
