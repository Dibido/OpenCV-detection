#ifndef CAMERA_H_
#define CAMERA_H_

// Libary
#include <stdint.h>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

// Namespace
using namespace cv;

class Camera
{
  public:
    Camera();
    ~Camera();

    void read();
    void display();

  private:
    VideoCapture vidCap;
    Mat currentFrame;
};

#endif