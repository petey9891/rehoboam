//#pragma once
//
//#include <Runnable.h>
//
//class SolidColor : public Runnable {
//public:
//    SolidColor(rgb_matrix::RGBMatrix* m, rgb_matrix::FrameCanvas* c);
//    virtual ~SolidColor() = default;
//
//    void run() override;
//    void setCommand(Command cmd) override;
//
//private:
//    float COLOR_STEP = 0.01f;
//
//    float brightness = 1.0f;
//    float expectedBrightness = 1.01f;
//    struct RGBColor {
//        float r;
//        float g;
//        float b;
//    };
//    RGBColor color;
//
//    enum State : uint8_t {
//        INCREASING,
//        DECREASING,
//        NONE
//    };
//    State state;
//};
