#include "pixy.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

int main(int argc, char *argv[])
{
    pixy ctx;
    ctx.setCameraBrightness(0xFF);
    ctx.setLamp(0x01, 0x00);
    ctx.setLED(0xFF, 0xFF, 0xFF);
    do
    {
        cv::imshow("test", ctx.read());

    } while (cv::waitKey(30) != 'q');
    return 0;
}
