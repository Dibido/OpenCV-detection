/**
 * @file shapedetector.h
 * @dibran
 * @brief shapedetector class for detecting shapes.
 * @version 0.1
 * @date 2019-02-17
 * 
 * @copyright Copyright (c) 2019
 */

#include <string>
#include <vector>

/**
 * @brief Enum for the different possible shapes
 */
enum shapes
{
  CIRCLE,
  HALFCIRCLE,
  SQUARE,
  RECTANGLE,
  TRIANGLE,
};

static const std::vector<std::string> shapestrings = 
{
  "circle",
  "halfcircle",
  "square",
  "rectangle",
  "trangle"
};

std::string ShapeToString(shapes aShape)
{
  return shapestrings[aShape];
}

shapes StringToShape(std::string aShapeString)
{
  for(size_t i = 0; i < shapestrings.size(); i++)
  {
    if(aShapeString == shapestrings[i])
    {
      return shapes(i);
    }
  }
}

/**
 * @brief Enum for the different possible colors
 */
enum colors
{
  RED,
  GREEN,
  BLUE,
  BLACK,
  YELLOW,
  WHITE,
  MAX
};

class shapedetector
{
  shapedetector();
  ~shapedetector();

  int detectShape(shapes aShape);
  int detecColor(shapes aShape);
};