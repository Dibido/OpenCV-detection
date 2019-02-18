/**
 * @file Camera.h
 * @Thijs
 * @brief Camera class for handling a camera
 * @version 0.1
 * @date 2019-02-18
 */
#ifndef CAMERA_H_
#define CAMERA_H_

class Camera
{
    public:

    private:
        VideoCapture vidCap(0);
        Mat currentFrame;
}

#endif