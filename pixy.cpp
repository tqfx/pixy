#include "pixy.h"

pixy::pixy()
{
    _image = cv::Mat(PIXY2_RAW_FRAME_HEIGHT, PIXY2_RAW_FRAME_WIDTH, CV_8UC3);
    _isok = 0;
    init();
}

pixy::~pixy()
{
    setLamp(0, 0);
    exit();
}

int pixy::init(void)
{
    int res = 0;
    if (_isok)
    {
        goto exit;
    }

    // Initialize Pixy2 Connection //
    res = _pixy.init();
    if (res < 0)
    {
        printf("Error\n");
        printf("pixy.init() returned %d\n", res);
        goto exit;
    }
#if !defined NDEBUG
    printf("Success\n");
#endif /* NDEBUG */
    _isok = 1;

    // Get Pixy2 Version information //
    res = _pixy.getVersion();
    if (res < 0)
    {
        printf("pixy.getVersion() returned %d\n", res);
        goto stop;
    }
#if !defined NDEBUG
    _pixy.version->print();
#endif /* NDEBUG */

stop:
    // need to call stop() befroe calling getRawFrame().
    // Note, you can call getRawFrame multiple times after calling stop().
    // That is, you don't need to call stop() each time.
    _pixy.m_link.stop();
exit:
    return 0;
}

void pixy::exit(void)
{
    if (_isok)
    {
        // Call resume() to resume the current program, otherwise Pixy will be left
        // in "paused" state.
        _pixy.m_link.resume();
    }
}

static void pixy_bgr(uint16_t const width, uint16_t const height, uint8_t const *const bayer, uint8_t *const res)
{
    uint8_t *pixel0, *pixel;
    uint32_t x, y, xx, yy, r, g, b;
    for (y = 0; y < height; ++y)
    {
        yy = y;
        if (yy == 0)
        {
            yy++;
        }
        else if (yy == (uint16_t)(height - 1))
        {
            yy--;
        }
        pixel0 = (uint8_t *)bayer + yy * width;
        for (x = 0; x < width; ++x)
        {
            xx = x;
            if (xx == 0)
            {
                xx++;
            }
            else if (xx == (uint16_t)(width - 1))
            {
                xx--;
            }
            pixel = pixel0 + xx;
            if (yy & 1)
            {
                if (xx & 1)
                {
                    r = *pixel;
                    g = (*(pixel - 1) + *(pixel + 1) + *(pixel + width) + *(pixel - width)) >> 2;
                    b = (*(pixel - width - 1) + *(pixel - width + 1) + *(pixel + width - 1) + *(pixel + width + 1)) >> 2;
                }
                else
                {
                    r = (*(pixel - 1) + *(pixel + 1)) >> 1;
                    g = *pixel;
                    b = (*(pixel - width) + *(pixel + width)) >> 1;
                }
            }
            else
            {
                if (xx & 1)
                {
                    r = (*(pixel - width) + *(pixel + width)) >> 1;
                    g = *pixel;
                    b = (*(pixel - 1) + *(pixel + 1)) >> 1;
                }
                else
                {
                    r = (*(pixel - width - 1) + *(pixel - width + 1) + *(pixel + width - 1) + *(pixel + width + 1)) >> 2;
                    g = (*(pixel - 1) + *(pixel + 1) + *(pixel + width) + *(pixel - width)) >> 2;
                    b = *pixel;
                }
            }
            uchar *const ptr = res + (x + y * width) * 3;
            ptr[0] = b;
            ptr[1] = g;
            ptr[2] = r;
        }
    }
}

int pixy::readbgr(uint8_t *ptr)
{
    if (_isok)
    {
        uint8_t *bayer;
        // grab raw frame, BGGR Bayer format, 1 byte per pixel
        _pixy.m_link.getRawFrame(&bayer);
        // convert Bayer frame to RGB frame
        pixy_bgr(PIXY2_RAW_FRAME_WIDTH, PIXY2_RAW_FRAME_HEIGHT, bayer, ptr);
    }
    return _isok - 1;
}

cv::Mat &pixy::read(void)
{
    if (_isok)
    {
        uint8_t *bayer;
        // grab raw frame, BGGR Bayer format, 1 byte per pixel
        _pixy.m_link.getRawFrame(&bayer);
        // convert Bayer frame to RGB frame
        pixy_bgr(PIXY2_RAW_FRAME_WIDTH, PIXY2_RAW_FRAME_HEIGHT, bayer, _image.ptr());
    }
    return _image;
}

int pixy::setLED(uint8_t r, uint8_t g, uint8_t b)
{
    if (_isok)
    {
        return _pixy.setLED(r, g, b);
    }
    return _isok - 1;
}

int pixy::setCameraBrightness(uint8_t brightness)
{
    if (_isok)
    {
        return _pixy.setCameraBrightness(brightness);
    }
    return _isok - 1;
}

int pixy::setLamp(uint8_t upper, uint8_t lower)
{
    if (_isok)
    {
        return _pixy.setLamp(upper, lower);
    }
    return _isok - 1;
}
