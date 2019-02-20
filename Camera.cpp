// Local
#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::read()
{
    if (vidCap.isOpened() == false)
    {
        std::cout << "Error: video capture not opened" << std::endl;
    }
    vidCap.read(currentFrame);
}

void Camera::display()
{
    namedWindow("Camera");
    imshow("Camera", currentFrame);
    moveWindow("Camera", 1000, 500);
}