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

    /// Constants
    const std::string EXIT_COMMAND = "exit";
    const std::string INTERACTIVE_MODE = "interactive";
    const std::string BATCH_MODE = "batch";

    const int INTERACTIVE_ARGCOUNT = 2;
    const int BATCH_ARGCOUNT = 3;

    if (argc > 1)
    {
        std::string imgPath = argv[1];
        Shapedetector shapeDetector; // create shape detector

        if (argc == INTERACTIVE_ARGCOUNT && fileExists(imgPath)) // shapedetector [image]
        {
            std::cout << "### cmdLine ###" << std::endl;
            shapeDetector.startCommandline();
        }
        else if (argc == INTERACTIVE_ARGCOUNT)
        {
            std::cout << "### webcam ###" << std::endl;
            shapeDetector.webcamMode(atoi(argv[1]));
        }
        else if (argc == BATCH_ARGCOUNT) // shapedetector [image] [batchfile]
        {
            std::cout << "### batch ###" << std::endl;
            shapeDetector.batchMode(argv[1], argv[2]);
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