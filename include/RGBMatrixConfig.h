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
        defaults.pwm_bits = 11;
        defaults.pwm_lsb_nanoseconds = 50;
        // defaults.panel_type = "FM6126A";
        defaults.rows = 64;
        defaults.cols = 192;
        defaults.chain_length = 1;
        defaults.parallel = 1;
        defaults.brightness = 50; // 60 is a good brightness for downtime
        // defaults.brightness = 70;

        runtime.drop_privileges = 0;
        runtime.gpio_slowdown = 1;
    }
};
