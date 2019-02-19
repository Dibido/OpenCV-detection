#ifndef CAMERA_H_
#define CAMERA_H_

// Libary
#include <stdint.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

// Constants
const uint8_t ESC_KEY = 27;

class Camera
{
  public:
    Camera(/* args */);
    ~Camera();

    void init();
    void startStream();

  private:
    VideoCapture vidCap(0);
    Mat currentFrame;
};

#endif