#pragma once

#include <Runnable.h>
#include <unistd.h>

class Christmas: public Runnable {
public:
    Christmas(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c);
    virtual ~Christmas() = default;

    void run() override;
    void setCommand(Command cmd) override;

private:
    uint32_t continuum = 0;
};
