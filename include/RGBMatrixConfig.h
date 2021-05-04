#pragma once

#include <rpi-led-matrix/led-matrix.h>

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

using namespace rgb_matrix;

struct RGBMatrixConfig {
    RGBMatrix::Options defaults;
    rgb_matrix::RuntimeOptions runtime;

    RGBMatrixConfig() {
        defaults.hardware_mapping = "adafruit-hat-pwm";
        defaults.led_rgb_sequence = "BGR";
        // defaults.pwm_lsb_nanoseconds = 50;
        // defaults.panel_type = "FM6126A";
        defaults.rows = 64;
        defaults.cols = 192;
        defaults.chain_length = 1;
        // defaults.brightness = 100; // 60 is a good brightness for downtime
        // defaults.brightness = 80;
        // defaults.brightness = 20;

        defaults.show_refresh_rate = true;
        

        runtime.drop_privileges = 0;
    }
};
