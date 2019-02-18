
const uint8_t ESC_KEY = 27;

Camera::init()
{
    if (vidCap.isOpened() == false)
    {
        std::cout << "Video capture not opened" << std::endl;
        return -1;
    }
}

Camera::startStream()
{
    while (vidCap.read(currentFrame))
    {
        namedWindow("CameraStream");
        imshow("CameraStream", currentFrame);
        moveWindow("CameraStream", 1000, 500);

        int pressedKey = waitKey(30);
        if (pressedKey == ESC_KEY)
            break;
    }
}
