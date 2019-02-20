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
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
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
  ALL,
  CIRCLE,
  HALFCIRCLE,
  SQUARE,
  RECTANGLE,
  TRIANGLE,
  UNKNOWNSHAPE
};

static const std::vector<std::string> SHAPESTRINGS =
{
  "alles",
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
  for (size_t i = 0; i < SHAPESTRINGS.size(); i++)
  {
    if (aShapeString == SHAPESTRINGS[i])
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
  for (size_t i = 0; i < COLORSTRINGS.size(); i++)
  {
    if (aColorString == COLORSTRINGS[i])
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
inline Scalar ScalarHSV2BGR(uchar H, uchar S, uchar V)
{
  Mat rgb;
  Mat hsv(1, 1, CV_8UC3, Scalar(H, S, V));
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
inline bool fileExists(const std::string &aFilePath)
{
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
    /**
     * @brief Detect a color in an image
     * @param aColor the color to detect
     * @return Mat a Mask with the current color filter
     */
    Mat detectColor(COLORS aColor);

    /**
     * @brief Detect a shape in an image
     * @param aShape the shape to detect
     * @return std::vector<Mat> The contours of the found shapes
     */
    std::vector<Mat> detectShape(SHAPES aShape);

    /**
     * @brief Set the X/Y/Area in the center of the shape
     * @param aImage The image to set the values on
     * @param aContour The contour to place the values in
     */
    void setShapeValues(Mat aImage, Mat aContour);

    /**
     * @brief Set the Time in the image
     * @param aImage The image to set the time in
     * @param aStartTime The start time
     * @param aEndTime The end time
     */
    void setTimeValue(Mat aImage, std::clock_t aStartTime, std::clock_t aEndTime);

    /**
     * @brief Draws the contours of a shape
     * @param aImage The image to draw on
     * @param aContour The contours to draw
     */
    void drawShapeContours(Mat aImage, Mat aContour);

    void setShapeFound(Mat aImage);

    // Variables
    std::string mImagePath;

    // Image matrices
    Mat mOriginalImage;
    Mat mDisplayImage;
    Mat mHSVImage;
    Mat mGreyImage;
    Mat mTresholdImage;
    Mat mMaskImage;
    Mat mApproxImage;

    // Current command values
    COLORS mCurrentColor;
    SHAPES mCurrentShape;

    // Calculation values
    Mat mCurrentMask;
    std::vector<Mat> mCurrentContours;
    Moments mCurrentMoments;

    // Shapecounter
    int mCurrentShapeCount;

    // Blur variables
    Size mGaussianKernelsize;

    // Timer variables
    std::clock_t mClockStart;
    std::clock_t mClockEnd;
    
    // Time position
    int mTimeXOffset;
    int mTimeYOffset;

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
    double mEpsilonMultiply;
    double mMinContourSize;
    double mMaxContourSize;

    // Text variables
    int mTextOffset;
    double mTextSize;
};

#endif