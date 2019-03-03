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

// Defines
#define SQUARE_CORNERCOUNT 4
#define TRIANGLE_CORNERCOUNT 3

/// Constants
const std::string EXIT_COMMAND = "exit";
const std::string INTERACTIVE_MODE = "interactive";
const std::string BATCH_MODE = "batch";

const int INTERACTIVE_ARGCOUNT = 2;
const int BATCH_ARGCOUNT = 3;
const char COMMENT_CHARACTER = '#';

// Enums
enum SHAPES
{
  ALL_SHAPES,
  CIRCLE,
  HALFCIRCLE,
  SQUARE,
  RECTANGLE,
  TRIANGLE,
  UNKNOWNSHAPE
};

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

// Strings
static const std::vector<std::string> SHAPESTRINGS =
    {
        "alles",
        "cirkel",
        "halfcirkel",
        "vierkant",
        "rechthoek",
        "driehoek",
        "onbekend"};
static const std::vector<std::string> COLORSTRINGS =
    {
        "rood",
        "groen",
        "blauw",
        "zwart",
        "geel",
        "wit",
        "onbekend"};

// Shape converters

/**
 * @brief Convert a shape to the string representation
 * 
 * @param aShape the shape
 * @return std::string the string representation
 */
inline std::string ShapeToString(SHAPES aShape)
{
  return SHAPESTRINGS[aShape];
}

/**
 * @brief Convert a string representation of a shape to a shape
 * 
 * @param aShapeString a string representation of a shape
 * @return SHAPES the shape
 */
inline SHAPES StringToShape(const std::string &aShapeString)
{
  SHAPES result = SHAPES::UNKNOWNSHAPE;

  for (size_t i = 0; i < SHAPESTRINGS.size(); i++)
  {
    if (aShapeString == SHAPESTRINGS.at(i))
    {
      result = SHAPES(i);
    }
  }

  if (result == SHAPES::UNKNOWNSHAPE)
  {
    std::cout << "Warning: \"" << aShapeString << "\" is not a valid shape" << std::endl;
  }

  return result;
}

// Color converters

/**
 * @brief Convert a color to the string representation
 * 
 * @param aColor the color to convert
 * @return std::string the string representation
 */
inline std::string ColorToString(COLORS aColor)
{
  return COLORSTRINGS[aColor];
}

/**
 * @brief convert a string representation of a color to a color
 * 
 * @param aColorString the string representation
 * @return COLORS the color
 */
inline COLORS StringToColor(const std::string &aColorString)
{
  COLORS result = COLORS::UNKNOWNCOLOR;

  for (size_t i = 0; i < COLORSTRINGS.size(); i++)
  {
    if (aColorString == COLORSTRINGS[i])
    {
      result = COLORS(i);
    }
  }

  if (result == COLORS::UNKNOWNCOLOR)
  {
    std::cout << "Warning: \"" << aColorString << "\" is not a valid color" << std::endl;
  }

  return result;
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

/**
 * @brief Shapedetector class
 */
class Shapedetector
{
public:
  Shapedetector();
  ~Shapedetector();

  /**
   * @brief Reset the images to the new captured image
   */
  void reset();
  /**
   * @brief Draw the data on the result image
   */
  void draw();
  /**
   * @brief Recognize the requested shape
   * 
   */
  void recognize();

  /**
   * @brief Function for handling the webcam mode
   * @param deviceId The webcam device Id
   */
  void webcamMode(int deviceId);
  /**
   * @brief Function for handling the batch mode
   * @param cameraId The camera device id
   * @param batchPath The path to the batch file to use
   */
  void batchMode(int cameraId, std::string batchPath);

  /**
   * @brief Set the image to use for recognicion
   * @param aImage the image to set
   */
  void setImage(Mat aImage);

  /**
   * @brief Shows the images
   * @return true The exit key was pressed
   * @return false The exit key was not pressed
   */
  bool showImages();
  /**
   * @brief Parses the current specification
   * @param aShapeCommand The command to parse
   * @return true The parsing was sucessfull
   * @return false the parsing failed
   */
  bool parseSpec(const std::string &aShapeCommand);
  
  /**
   * @brief Open the camera to make it ready for capturing
   * @param cameraId The id of the camera
   */
  void initCamera(int cameraId);

  /**
   * @brief Handles the real-time detection algorithm
   */
  void detectRealtime();

  /**
   * @brief The capture object for handling the webcam
   */
  VideoCapture mVidCap;

  /**
     * @brief Handles a single shape command
     * @param aShapeCommand 
     */
  void handleShapeCommand(const std::string &aShapeCommand);

  // Image matrices to show
  Mat mOriginalImage;      // original
  Mat mBrightenedRgbImage; // brightness image
  Mat mBlurredImage;       // blurred image
  Mat mMaskImage;          // color filtered image
  Mat mDisplayImage;       // image with shape outlines

private:
  // Program variables
  std::string mImagePath;
  std::string mCurrentShapeCommand;

  // Image matrices
  Mat mHSVImage;
  Mat mGreyImage;
  Mat mTresholdImage;
  Mat mApproxImage;

  // Slider values
  int mBlurSliderValue;
  int mContrastSliderValue;
  int mNoiseSliderValue;
  int mMinRatioSliderValue;
  int mMaxRatioSliderValue;

  // Slider ranges
  int mBlurSliderRange;
  int mContrastSliderRange;
  int mNoiseSliderRange;

  int mMinRatioSliderRange;
  int mMaxRatioSliderRange;

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
  double mMinSquareRatio;
  double mMaxSquareRatio;

  // Contour settings
  int mContourCenterMargin;
  double mEpsilonMultiply;
  double mMinContourSize;
  double mMaxContourSize;
  double mMinHalfCirclePercentage;
  double mMaxHalfCirclePercentage;

  // Text variables
  int mTextOffset;
  double mTextSize;

  // Draw size
  unsigned int mScreenDrawWidth;
  unsigned int mScreenDrawHeight;

  /**
   * @brief Initialize the class values
   */
  void initializeValues();

  /**
 * @brief Detect a color in an image
 * @param aColor the color to detect
 * @return Mat a Mask with the current color filter
 */
  Mat detectColor(COLORS aColor, Mat aImage);

  /**
     * @brief Detect a shape in an image
     * @param aShape the shape to detect
     * @return std::vector<Mat> The contours of the found shapes
     */
  std::vector<Mat> detectShape(SHAPES aShape, Mat aShapeMask);

  /**
   * @brief finds the squares in an image
   * @param aContours the contours in the image
   */
  void detectSquares(std::vector<Mat> aContours);

  /**
   * @brief finds the rectangles in an image
   * @param aContours the contours in the image
   */
  void detectRectangles(std::vector<Mat> aContours);

  /**
   * @brief finds the triangles in an image
   * @param aContours the contours in the image
   */
  void detectTriangles(std::vector<Mat> aContours);

  /**
   * @brief finds the circles in an image
   * @param aContours the contours in the image
   */
  void detectCircles(std::vector<Mat> aContours);

  /**
   * @brief Checks whether the contour is within the min and max contourSize
   * @param aContour the contour to check
   * @return whether the contour is within the range
   */
  bool contourSizeAllowed(Mat aContour) const;

  /**
   * @brief finds the halfcircles in an image
   * @param aContours the contours in the image
   */
  void detectHalfCircles(std::vector<Mat> aContours);

  /**
   * @brief finds the halfcircles in an image using the houghcircles algorithm
   * @param aContours the contours in the image
   */
  void detectHalfCirclesHough(std::vector<Mat> aContours);

  /**
   * @brief Set the shape command in the image
   * @param aImage the image to set the command in
   */
  void setShapeCommand(Mat aImage);

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
  static void drawShapeContours(Mat aImage, Mat aContour);

  /**
     * @brief Set the count of shapes found
     * @param aImage the image to set the count on
     */
  void setShapeFound(Mat aImage);

  /**
   * @brief Get the center point of a contour
   */
  static Point getContourCenter(Mat aContour);

  /**
   * @brief remove the shapes where the center point is too close to another shape
   * @param aContours the contours to check
   */
  void removeCloseShapes(std::vector<Mat> &aContours) const;

  /**
   * @brief Callback for setting the slider values in the program
   */
  static void onChange(int, void *);

  /**
   * @brief filters the noise from the image
   */
  Mat removeNoise(Mat aImage);

  /**
   * @brief Print the data from the detection to the console
   */
  void printDetectionData();
};

#endif