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

// void interactiveMode(std::string imgPath)
// {
// int r = 50;
// int range = 100;
// createTrackbar("R", "mask", &r, range);

// int pressedKey = waitKey(0);
// if (pressedKey = ESC_KEY) { /* do something */ }

/*
	Program flow:
	1. set color (black, white, red, green, blue, yellow)
	2. set shape (circle, halfCircle, square, rectangle, triangle)
	*/
// }

void batchMode()
{
    // TODO implement
}

int main(int argc, char **argv)
{
    std::string imgPath = argv[1];

    if (argc >= INTERACTIVE_ARGCOUNT && fileExists(imgPath)) // if 1 argument
    {
        Shapedetector shapeDetector(imgPath); // create shape detector

        // Start gui
        std::cout << "Entered interactive mode" << std::endl;
        std::cout << "Please enter [kleur] [vorm]" << std::endl;

        while (true)
        {
            std::cout << "> ";
            std::string command;
            getline(std::cin, command); // Get command

            if (command == EXIT_COMMAND)
            {
                break;
            }
            else
            {
                shapeDetector.handleShapeCommand(command); // Start algorithm
            }
        }
    }
    else if (argc == BATCH_ARGCOUNT) // shapedetector [image] [batchfile]
    {
        std::cout << "Started in batchmode" << std::endl;
        if (fileExists(argv[2]))
        {
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
        std::cout << "Invalid arguments or filepath, usage: \n\
      Interactive mode: shapedetector [image]\n\
      Batch mode: shapedetector [image] [batchfile]\n"
                  << std::endl;
        exit(0);
    }

    return 0;
}