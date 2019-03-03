/// Library
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <stdlib.h>

/// Local
#include "Shapedetector.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        Shapedetector shapeDetector; // create shape detector

        if (argc == INTERACTIVE_ARGCOUNT)
        {
            shapeDetector.webcamMode(atoi(argv[1]));
        }
        else if (argc == BATCH_ARGCOUNT) // shapedetector [image] [batchfile]
        {
            shapeDetector.batchMode(atoi(argv[1]), argv[2]);
        }
    }
    else
    {
        std::cout << "Error: invalid arguments or filepath, usage:" << std::endl;
        std::cout << "\tWebcam mode:\t\tshapedetector [device id]" << std::endl;
        std::cout << "\tBatch mode:\t\tshapedetector [device id] [batchfile]" << std::endl;
    }

    return 0;
}