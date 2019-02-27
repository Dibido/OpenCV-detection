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

/// Constants
const std::string EXIT_COMMAND = "exit";
const int INTERACTIVE_ARGCOUNT = 2;
const int BATCH_ARGCOUNT = 3;
const char COMMENT_CHARACTER = '#';

int main(int argc, char **argv)
{
  std::string imgPath = argv[1];

  if (argc == INTERACTIVE_ARGCOUNT && fileExists(imgPath)) // shapedetector [image]
  {
    Shapedetector shapeDetector(imgPath, false); // create shape detector

    // Start GUI
    std::cout << "### Interactive mode ###" << std::endl;
    std::cout << "Please enter [vorm] [kleur]" << std::endl;

    while (true)
    {
      std::cout << "> ";
      std::string command;
      getline(std::cin, command); // Get command

      if (command != EXIT_COMMAND)
      {
        shapeDetector.handleShapeCommand(command); // Start algorithm
      }
      else if (command == EXIT_COMMAND)
      {
        std::cout << "Closing program.." << std::endl;
        break;
      }
      else
      {
        break;
      }
    }
  }
  else if (argc == INTERACTIVE_ARGCOUNT)
  {
    // Set pretake cam framecount
    const int WEBCAM_START_IMAGES = 5;
    // Check input
    int deviceId = atoi(argv[1]);
    // Read image from webcam
    VideoCapture cap;
    Mat capturedImage;
    cap.open(deviceId);
    if(cap.isOpened())
    {
      for(size_t i = 0; i < WEBCAM_START_IMAGES; i++)
      {
        cap.grab();
        cap.retrieve(capturedImage);
      }
      cap.retrieve(capturedImage);
      imwrite("webcamblocks1.png", capturedImage);
    }
    // Start webcam mode
    std::cout << "### Webcam mode ###" << std::endl;
    std::cout << "Please enter [vorm] [kleur]" << std::endl;

    Shapedetector shapeDetector(capturedImage, false); // create shape detector

    while (true)
    {
      std::cout << "> ";
      std::string command;
      getline(std::cin, command); // Get command

      if (command != EXIT_COMMAND)
      {
        shapeDetector.handleShapeCommand(command); // Start algorithm
      }
      else if (command == EXIT_COMMAND)
      {
        std::cout << "Closing program.." << std::endl;
        break;
      }
      else
      {
        break;
      }
    }
  }
  else if (argc == BATCH_ARGCOUNT && fileExists(imgPath)) // shapedetector [image] [batchfile]
  {
    Shapedetector shapeDetector(imgPath, true); // create shape detector
    // Start batchmode
    std::cout << "### Batch mode ###" << std::endl;
    if (fileExists(argv[2]))
    {
      std::string line;
      std::ifstream batchFile(argv[2]);
      while (std::getline(batchFile, line))
      {
        // Ignore comments
        if(line.at(0) != COMMENT_CHARACTER)
        {
          std::cout << line << std::endl;
          shapeDetector.handleShapeCommand(line);
        }
      }
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
      Webcam mode: shapedetector [device id]\n\
      Interactive mode: shapedetector [image]\n\
      Batch mode: shapedetector [image] [batchfile]\n"
              << std::endl;
    exit(0);
  }
  return 0;
}