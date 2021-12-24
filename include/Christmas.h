#pragma once

#include <Runnable.h>

class Christmas: public Runnable {
public:
    Christmas(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c);
    virtual ~Christmas() = default;

    void run() override;
    void setCommand(Command cmd) override;
}