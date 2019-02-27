/// Library
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>

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
    while (true)
    {
      std::cout << "Please enter [vorm] [kleur]" << std::endl;
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
      //TODO: Implement batch mode
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
      Interactive mode: shapedetector [image]\n\
      Batch mode: shapedetector [image] [batchfile]\n"
              << std::endl;
    exit(0);
  }
  return 0;
}