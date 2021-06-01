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
    uint8_t dataLength;
    // uint8_t* data[];
    std::vector<uint8_t> data;

    // Command(CommandType t, uint8_t l, uint8_t* d)
    //     : type { type }, dataLength { l }, data { d }
    // {}
};
