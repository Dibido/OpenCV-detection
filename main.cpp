#include <opencv2/opencv.hpp>
#include <iostream>

#define BATCHMODE_ARGS 2

int main(int argc, char** argv)
{
  // Batch mode
  if(argc == BATCHMODE_ARGS)
  {
    std::cout << argv[1] << std::endl;
    // TODO: process batch file
  }
  else
  {
    
    //TODO: read command
  }
  return 0;
}