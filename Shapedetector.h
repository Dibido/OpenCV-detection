/**
 * @file shapedetector.h
 * @dibran
 * @brief shapedetector class for detecting shapes.
 * @version 0.1
 * @date 2019-02-17
 */
#ifndef SHAPE_DETECTOR_H_
#define SHAPE_DETECTOR_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

/**
 * @brief Enum for the different possible shapes
 */
enum SHAPES
{
  CIRCLE,
  HALFCIRCLE,
  SQUARE,
  RECTANGLE,
  TRIANGLE,
};

static const std::vector<std::string> SHAPESTRINGS = 
{
  "circle",
  "halfcircle",
  "square",
  "rectangle",
  "trangle"
};

inline std::string ShapeToString(SHAPES aShape)
{
  return SHAPESTRINGS[aShape];
}

inline SHAPES StringToShape(std::string aShapeString)
{
  for(size_t i = 0; i < SHAPESTRINGS.size(); i++)
  {
    if(aShapeString == SHAPESTRINGS[i])
    {
      return SHAPES(i);
    }
  }
}

/**
 * @brief Enum for the different possible colors
 */
enum COLORS
{
  RED,
  GREEN,
  BLUE,
  BLACK,
  YELLOW,
  WHITE
};

static const std::vector<std::string> COLORSTRINGS = 
{
  "red",
  "green",
  "blue",
  "black",
  "yellow",
  "white"
};

inline std::string ColorToString(COLORS aColor)
{
  return COLORSTRINGS[aColor];
}

inline COLORS StringToColor(std::string aColorString)
{
  for(size_t i = 0; i < COLORSTRINGS.size(); i++)
  {
    if(aColorString == COLORSTRINGS[i])
    {
      return COLORS(i);
    }
  }
}

inline bool fileExists (const std::string& aFilePath) {
  std::ifstream f(aFilePath.c_str());
  return f.good();
}

class Shapedetector
{
  public:
    Shapedetector(std::string aImageFilePath);
    ~Shapedetector();

    /**
     * @brief 
     * @param aShapeCommand 
     */
    void handleShapeCommand(const std::string& aShapeCommand);

  private:
    int detectShape(SHAPES aShape);
    int detectColor(COLORS aShape);

    COLORS mCurrentColor;
    SHAPES mCurrentShape;
    std::string mImagePath;
    Mat mOriginalImage;
};

#endif