#pragma once

#include "opencv2/core.hpp"
#include "src/host/libpixyusb2/include/libpixyusb2.h"

class pixy
{
    cv::Mat _image;
    Pixy2 _pixy;
    int _isok;

public:
    int setLED(uint8_t r, uint8_t g, uint8_t b);
    int setCameraBrightness(uint8_t brightness);
    int setLamp(uint8_t upper, uint8_t lower);
    int readbgr(uint8_t *bgr);
    cv::Mat &read();
    void exit();
    int init();
    int isok();
    ~pixy();
    pixy();
};
