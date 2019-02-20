/// Library
#include <opencv2/opencv.hpp>
#include <iostream>
#include <memory>

/// Local
#include "Shapedetector.h"
#include "Camera.h"

/// Defines
#define INTERACTIVE_ARGCOUNT 2
#define BATCHMODE_ARGCOUNT 3

/// Constants
const std::string EXIT_COMMAND = "exit";

/// Global variables
std::shared_ptr<Shapedetector> detector;

int main(int argc, char **argv)
{
    // Interactive mode
    if (argc >= INTERACTIVE_ARGCOUNT)
    {
        if (fileExists(argv[1]))
        {
            detector = std::make_shared<Shapedetector>(argv[1]);
        }
        else
        {
            std::cout << "ERROR - Image File does not exist." << std::endl;
            exit(0);
        }
    }
    // Batch mode
    if (argc == BATCHMODE_ARGCOUNT)
    {
        std::cout << "Started in batchmode" << std::endl;
        if (fileExists(argv[2]))
        {
            //TODO: process batch file
        }
        else
        {
            std::cout << "ERROR - Batchfile does not exist." << std::endl;
            exit(0);
        }
    }
    // Interactive mode
    else if (argc == INTERACTIVE_ARGCOUNT)
    {
        std::cout << "Entered interactive mode" << std::endl
                  << "> ";
        std::string aCommand;
        while (aCommand != EXIT_COMMAND)
        {
            getline(std::cin, aCommand);
            detector->handleShapeCommand(aCommand);
            std::cout << "> ";
        }
    }
    else
    {
        std::cout << "Invalid arguments. \n\
      Usage - Interactive: shapedetector [image]\n\
      Batch: shapedetector [image] [batchfile]\n"
                  << std::endl;
        exit(0);
    }
    return 0;
}