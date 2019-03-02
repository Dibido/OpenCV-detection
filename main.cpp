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