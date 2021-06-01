#pragma once
#include <stdint.h>
#include <vector>

enum CommandType: uint8_t {
    DisplayOn,
    DisplayOff,
    Brightness,
    ColorPulseMode,
    RehoboamMode,
    StaticColor
};

struct Command {
    CommandType type;
    std::vector<uint8_t> data;
};
