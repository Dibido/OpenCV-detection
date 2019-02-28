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
#include "Camera.h"

int main(int argc, char **argv)
{
    // COMMANDS
    // ./shapedetector interactive [device id]
    // ./shapedetector batch [file]

    // EXAMPLES
    // ./shapedetector interactive 1
    // ./shapedetector batch ../batch.txt

    if (argc > 1)
    {
        std::string imgPath = argv[1];
        Shapedetector shapeDetector(argv[1]); // create shape detector

        if (argc == INTERACTIVE_ARGCOUNT && fileExists(imgPath)) // shapedetector [image]
        {
            shapeDetector.startCommandline();
        }
        else if (argc == INTERACTIVE_ARGCOUNT)
        {
            shapeDetector.webcamMode(atoi(argv[1]));
        }
        else if (argc == BATCH_ARGCOUNT && fileExists(imgPath)) // shapedetector [image] [batchfile]
        {
            shapeDetector.batchMode(argv[2]);
        }
    }
    else
    {
        std::cout << "Error: invalid arguments or filepath, usage:" << std::endl;
        std::cout << "\tWebcam mode:\t\tshapedetector [device id]" << std::endl;
        std::cout << "\tInteractive mode:\tshapedetector [image]" << std::endl;
        std::cout << "\tBatch mode:\t\tshapedetector [image] [batchfile]" << std::endl;
    }

    return 0;
}