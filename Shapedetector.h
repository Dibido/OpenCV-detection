/**
 * @file shapedetector.h
 * @dibran
 * @brief shapedetector class for detecting shapes.
 * @version 0.1
 * @date 2019-02-17
 */
#ifndef SHAPE_DETECTOR_H_
#define SHAPE_DETECTOR_H_

// Library
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

// Local
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

// Namespace
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
  UNKNOWNSHAPE
};

static const std::vector<std::string> SHAPESTRINGS = 
{
  "cirkel",
  "halfcirkel",
  "vierkant",
  "rechthoek",
  "driehoek"
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
  return SHAPES::UNKNOWNSHAPE;
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
  WHITE,
  UNKNOWNCOLOR
};

static const std::vector<std::string> COLORSTRINGS = 
{
  "rood",
  "groen",
  "blauw",
  "zwart",
  "geel",
  "wit"
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
  return COLORS::UNKNOWNCOLOR;
}
/**
 * @brief Convert an HSV scalar to BGR
 * 
 * @param H Hue value
 * @param S Saturation value
 * @param V Value value
 * @return Scalar BGR Scalar
 */
inline Scalar ScalarHSV2BGR(uchar H, uchar S, uchar V) {
    Mat rgb;
    Mat hsv(1,1, CV_8UC3, Scalar(H,S,V));
    cvtColor(hsv, rgb, CV_HSV2BGR);
    return Scalar(rgb.data[0], rgb.data[1], rgb.data[2]);
}

/**
 * @brief Check whether a file exists
 * 
 * @param aFilePath The path to the file to check
 * @return true file exists
 * @return false file does not exist
 */
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
     * @brief Handles a single shape command
     * @param aShapeCommand 
     */
    void handleShapeCommand(const std::string& aShapeCommand);

  private:
    // Functions
    Mat detectColor(COLORS aShape);
    std::vector<Mat> detectShape(SHAPES aShape);
    void setShapeValues(Mat aContour);

    // Variables
    std::string mImagePath;

    Mat mOriginalImage;
    Mat mHSVImage;
    Mat mGreyImage;
    Mat mTresholdImage;
    Mat mMaskImage;
    Mat mApproxImage;

    COLORS mCurrentColor;
    SHAPES mCurrentShape;

    Moments mCurrentMoments;
    std::vector<Mat> mCurrentContours;
    Mat mCurrentMask;
    // Calibration variables
    // Color limits (Min / Max)
    Scalar mBlueLimits[2];
    Scalar mGreenLimits[2];
    Scalar mRedLimits[4]; // 4 values for 0-10 and 170-180
    Scalar mBlackLimits[2];
    Scalar mYellowLimits[2];
    Scalar mWhiteLimits[2];

    // Treshold values
    int mMinTreshold;
    int mMaxTreshold;
    ThresholdTypes mTresholdType;

    // Contour settings
    int mMinContourSize;
    int mTextOffset;
    double mTextSize;
};

#endif