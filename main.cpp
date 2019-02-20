/// Library
#include <opencv2/opencv.hpp>
#include <iostream>
#include <memory>

/// Local
#include "Shapedetector.h"
#include "Camera.h"

/// Constants
const std::string EXIT_COMMAND = "exit";
const int INTERACTIVE_ARGCOUNT = 2;
const int BATCH_ARGCOUNT = 3;

/// Global variables
std::shared_ptr<Shapedetector> detector;

void interactiveMode(std::string imgPath)
{
    detector = std::make_shared<Shapedetector>(imgPath); // start shapedetector

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

void batchMode()
{
}

int start()
{
    // interactive
    // batch
    // error

    
}

/// Error codes to be returned by functions
enum ErrorCode
{
    ERROR,
    INVALID_FILE_PATH

    //     std::cout << "ERROR - Image File does not exist." << std::endl;
    //     std::cout << "ERROR - Batchfile does not exist." << std::endl;
    // std::cout << "Invalid arguments. \n\""
};


int main(int argc, char **argv)
{
    std::string imgPath = argv[1];

    if (argc >= INTERACTIVE_ARGCOUNT) // shapedetector [image]
    {
        if (fileExists(imgPath)) // file found
        {
            interactiveMode(imgPath);
        }
        else // error, no file
        {
            std::cout << "ERROR - Image File does not exist." << std::endl;
            exit(0);
        }
    }
    else if (argc == BATCH_ARGCOUNT) // shapedetector [image] [batchfile]
    {
        std::cout << "Started in batchmode" << std::endl;
        if (fileExists(argv[2]))
        {
            //TODO: process batch file
            batchMode();
        }
        else
        {
            std::cout << "ERROR - Batchfile does not exist." << std::endl;
            exit(0);
        }
    }
    else
    {
        std::cout << "Invalid arguments, usage: \n\
      Interactive mode: shapedetector [image]\n\
      Batch mode: shapedetector [image] [batchfile]\n"
                  << std::endl;
        exit(0);
    }

    return 0;
}