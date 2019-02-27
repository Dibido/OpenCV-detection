// Local
#include "Shapedetector.h"

// Constructor
Shapedetector::Shapedetector(std::string aImageFilePath, bool isBatchMode) : mImagePath(aImageFilePath), mBatchMode(isBatchMode)
{
  // Store origininal image
  mOriginalImage = imread(mImagePath);
  mOriginalImage.copyTo(mDisplayImage);
  mOriginalImage.copyTo(mTresholdImage);

  // Convert to necessary formats
  cvtColor(mOriginalImage, mGreyImage, CV_BGR2GRAY);
  cvtColor(mOriginalImage, mHSVImage, CV_BGR2HSV);

  // Set the calibration variables
  mBlurSliderValue = 0;
  mContrastSliderValue = 0;
  mNoiseSliderValue = 0;

  mScreenDrawWidth = 500;
  mScreenDrawHeight = mScreenDrawWidth * 1080 / 1920;

  // Set the blur variables
  mGaussianKernelsize = Size(3, 3);

  // Set the treshold variables
  mMinTreshold = 120;
  mMaxTreshold = 255;
  mTresholdType = ThresholdTypes::THRESH_BINARY;

  // Set the Contours variables
  mContourCenterMargin = 30;
  mCurrentShapeCount = 0;
  mEpsilonMultiply = 0.04;
  mMinContourSize = 500.0;
  mMaxContourSize = 30000.0;
  mTextOffset = 20;
  mTextSize = 0.5;

  // Set the timer variables
  mTimeXOffset = 20;
  mTimeYOffset = 20;

  // Set the color limits for color detection [0] = Min, [1] = Max
  mBlueLimits[0] = Scalar(85, 20, 20);
  mBlueLimits[1] = Scalar(135, 255, 255);
  mGreenLimits[0] = Scalar(40, 20, 20);
  mGreenLimits[1] = Scalar(90, 255, 255);
  mRedLimits[0] = Scalar(0, 60, 60);
  mRedLimits[1] = Scalar(10, 255, 255);
  mRedLimits[2] = Scalar(170, 60, 60);
  mRedLimits[3] = Scalar(180, 255, 255);
  mBlackLimits[0] = Scalar(0, 0, 0);
  mBlackLimits[1] = Scalar(255, 100, 100);
  mYellowLimits[0] = Scalar(20, 40, 30);
  mYellowLimits[1] = Scalar(40, 255, 255);
  mWhiteLimits[0] = Scalar(10, 40, 30); // Is woodcolor instead of white
  mWhiteLimits[1] = Scalar(20, 255, 255);
}

// Destructor
Shapedetector::~Shapedetector()
{
}

// Shape detectors "main"
void Shapedetector::handleShapeCommand(const std::string& aShapeCommand)
{
  // Parse command
  std::size_t delimiterPos = aShapeCommand.find(' ');
  std::string shapeStr = aShapeCommand.substr(0, delimiterPos);
  std::string colorStr = aShapeCommand.substr(delimiterPos + 1);

  // Convert strings
  mCurrentColor = StringToColor(colorStr);
  mCurrentShape = StringToShape(shapeStr);

  if(mCurrentColor == COLORS::UNKNOWNCOLOR || mCurrentShape == SHAPES::UNKNOWNSHAPE)
  {
    std::cout << "Invalid command" << std::endl;
  }
  else
  {
    // Reset values
    mOriginalImage.copyTo(mDisplayImage);
    mOriginalImage.copyTo(mTresholdImage);
    mCurrentShapeCount = 0;

    recognize(); // run algorithm
    if(!mBatchMode)
    {
      draw(); // draw results
    }
    else
    {
      printDetectionData(); // Print data
    }
  }
}

void Shapedetector::printDetectionData()
{
  printTimeValue(mClockStart, mClockEnd);
  printShapeFound();
}

void Shapedetector::printTimeValue(std::clock_t aStartTime, std::clock_t aEndTime)
{
  std::cout << std::fixed << std::setprecision(2) << "\tCPU time used:\t" << 1000.0 * ((double)aEndTime - (double)aStartTime) / CLOCKS_PER_SEC << " ms" << std::endl;
}

void Shapedetector::printShapeFound()
{
  const std::string shapeCountText = std::to_string(mCurrentShapeCount) + " " + ShapeToString(mCurrentShape);
  std::cout << "\tShapes found:\t" << shapeCountText << std::endl;
}

// Starts the detection algorithm
void Shapedetector::recognize()
{
  mClockStart = std::clock();              // Start timer

  // Apply filters
  // Brighten
  Mat brightenedBGRImage;
  Mat brightenedHSVImage;
  mOriginalImage.convertTo(brightenedBGRImage, -1, 1, 40);
  cvtColor(brightenedBGRImage, brightenedHSVImage, COLOR_BGR2HSV);
  // imshow("brightened", brightenedBGRImage);
  // Blur
  // Mat blurredHSVImage;
  // GaussianBlur(brightenedHSVImage, blurredHSVImage, Size(3, 3), 0);
  // Filter color
  mMaskImage = detectColor(mCurrentColor, brightenedHSVImage);
  // Remove noise
  Mat removedNoise = removeNoise(mMaskImage);
  // Detect shapes
  detectShape(mCurrentShape, removedNoise);
  
  mClockEnd = std::clock();               // Stop timer
}

// Draws the windows and text
void Shapedetector::draw()
{
  setTimeValue(mDisplayImage, mClockStart, mClockEnd); // draw durations
  setShapeFound(mDisplayImage);                        // draw found shapes

  // Show original
  namedWindow("Original", WINDOW_NORMAL);
  imshow("Original", mOriginalImage);
  moveWindow("Original", 0, 0);
  resizeWindow("Original", mScreenDrawWidth, mScreenDrawHeight);

  // Show mask (optional)
  namedWindow("Mask", WINDOW_NORMAL);
  imshow("Mask", mMaskImage);
  moveWindow("Mask", mScreenDrawWidth, 0);
  resizeWindow("Mask", mScreenDrawWidth, mScreenDrawHeight);

  // Show result
  namedWindow("Result", WINDOW_NORMAL);
  imshow("Result", mDisplayImage);
  moveWindow("Result", mScreenDrawWidth * 2, 0);
  resizeWindow("Result", mScreenDrawWidth, mScreenDrawHeight);

  int blur = 0;
  int contrast = 0;
  int noise = 0;
  namedWindow("Sliders");
  createTrackbar("Blur", "Sliders", &blur, 255, onChange);
  createTrackbar("Contrast", "Sliders", &contrast, 255, onChange);
  createTrackbar("Noise", "Sliders", &noise, 255, onChange);
  moveWindow("Sliders", 0, mScreenDrawHeight);

  waitKey(0);
}

<<<<<<< HEAD
void Shapedetector::setShapeFound(Mat aImage)
{
  const std::string shapeCountText = std::to_string(mCurrentShapeCount) + " " + ShapeToString(mCurrentShape);

  std::cout << "\tShapes found:\t" << shapeCountText << std::endl;

  putText(aImage, shapeCountText, Point(mTimeXOffset, (mTimeYOffset * 2)), FONT_HERSHEY_SIMPLEX, mTextSize, Scalar(0, 0, 0), 1);
}

Mat Shapedetector::detectColor(COLORS aColor)
{
  Mat resultMask;
  Mat tempMask;
  Mat resultImage;

  switch (aColor)
  {
  case COLORS::BLUE:
  {
    inRange(mHSVImage, mBlueLimits[0], mBlueLimits[1], resultMask);
    break;
  }
  case COLORS::GREEN:
  {
    inRange(mHSVImage, mGreenLimits[0], mGreenLimits[1], resultMask);
    break;
  }
  case COLORS::RED:
  {
    inRange(mHSVImage, mRedLimits[0], mRedLimits[1], resultMask);
    inRange(mHSVImage, mRedLimits[2], mRedLimits[3], tempMask);
    bitwise_or(resultMask, tempMask, resultMask);
    break;
  }
  case COLORS::BLACK:
  {
    inRange(mHSVImage, mBlackLimits[0], mBlackLimits[1], resultMask);
    break;
  }
  case COLORS::YELLOW:
  {
    inRange(mHSVImage, mYellowLimits[0], mYellowLimits[1], resultMask);
    break;
  }
  case COLORS::WHITE:
  {
    inRange(mHSVImage, mWhiteLimits[0], mWhiteLimits[1], resultMask);
    break;
  }
  case COLORS::UNKNOWNCOLOR:
  {
    std::cout << "Error: unknown color = " << aColor << std::endl;
    break;
  }
  }
  resultMask.copyTo(mCurrentMask);
  mOriginalImage.copyTo(resultImage, resultMask);
  return resultImage;
}

Point getContourCenter(Mat aContour)
{
  //Calculate center
  Moments currentmoments = moments(aContour);
  int centerX = (int)(currentmoments.m10 / currentmoments.m00);
  int centerY = (int)(currentmoments.m01 / currentmoments.m00);
  return Point(centerX, centerY);
}

void Shapedetector::removeCloseShapes(std::vector<Mat> &aContours)
{
  Point currentCenter;
  Point compareCenter;
  for (int i = 0; i < aContours.size(); i++)
  {
    //Calculate center
    currentCenter = getContourCenter(aContours.at(i));
    //Remove duplicates
    for (int j = 0; j < aContours.size(); j++)
    {
      if (j != i) // Not the same shape
      {
        compareCenter = getContourCenter(aContours.at(j));
        int Xdiff = abs(currentCenter.x - compareCenter.x);
        int Ydiff = abs(currentCenter.y - compareCenter.y);
        //Shape is too close
        if (Xdiff <= mContourCenterMargin && Ydiff <= mContourCenterMargin)
        {
          aContours.erase(aContours.begin() + j);
        }
      }
    }
  }
}

std::vector<Mat> Shapedetector::detectShape(SHAPES aShape)
{
  GaussianBlur(mCurrentMask, mCurrentMask, mGaussianKernelsize, BorderTypes::BORDER_DEFAULT);
  adaptiveThreshold(mCurrentMask, mTresholdImage, 255, AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C, mTresholdType, 5, 2);
  findContours(mTresholdImage, mCurrentContours, CV_RETR_TREE, CHAIN_APPROX_NONE);
  removeCloseShapes(mCurrentContours);
  switch (aShape)
  {
  case SHAPES::ALL:
  {
    for (int i = 0; i < mCurrentContours.size(); i++)
    {
      double epsilon = mEpsilonMultiply * arcLength(mCurrentContours.at(i), true);
      approxPolyDP(mCurrentContours.at(i), mApproxImage, epsilon, true);
      if (contourArea(mCurrentContours.at(i)) < mMinContourSize || contourArea(mCurrentContours.at(i)) > mMaxContourSize)
      {
        //Ignore small or huge shapes
      }
      else
      {
        mCurrentShapeCount++;
        drawShapeContours(mDisplayImage, mCurrentContours.at(i));
        setShapeValues(mDisplayImage, mCurrentContours.at(i));
      }
    }
    break;
  }
  case SHAPES::SQUARE:
  {
    for (int i = 0; i < mCurrentContours.size(); i++)
    {
      double epsilon = mEpsilonMultiply * arcLength(mCurrentContours.at(i), true);
      approxPolyDP(mCurrentContours.at(i), mApproxImage, epsilon, true);
      if (mApproxImage.size().height == 4)
      {
        if (contourArea(mCurrentContours.at(i)) < mMinContourSize || contourArea(mCurrentContours.at(i)) > mMaxContourSize)
        {
          //Ignore small or huge shapes
        }
        else
        {
          mCurrentShapeCount++;
          drawShapeContours(mDisplayImage, mCurrentContours.at(i));
          setShapeValues(mDisplayImage, mCurrentContours.at(i));
        }
      }
    }
    break;
  }
  case SHAPES::RECTANGLE:
  {
    for (int i = 0; i < mCurrentContours.size(); i++)
    {
      double epsilon = mEpsilonMultiply * arcLength(mCurrentContours.at(i), true);
      approxPolyDP(mCurrentContours.at(i), mApproxImage, epsilon, true);
      if (mApproxImage.size().height == 4)
      {
        if (contourArea(mCurrentContours.at(i)) < mMinContourSize || contourArea(mCurrentContours.at(i)) > mMaxContourSize)
        {
          //Ignore small or huge shapes
        }
        else
        {
          mCurrentShapeCount++;
          drawShapeContours(mDisplayImage, mCurrentContours.at(i));
          setShapeValues(mDisplayImage, mCurrentContours.at(i));
        }
      }
    }
    break;
  }
  case SHAPES::TRIANGLE:
  {
    for (int i = 0; i < mCurrentContours.size(); i++)
    {
      double epsilon = mEpsilonMultiply * arcLength(mCurrentContours.at(i), true);
      approxPolyDP(mCurrentContours.at(i), mApproxImage, epsilon, true);
      if (mApproxImage.size().height == 3)
      {
        if (contourArea(mCurrentContours.at(i)) < mMinContourSize || contourArea(mCurrentContours.at(i)) > mMaxContourSize)
        {
          //Ignore small or huge shapes
        }
        else
        {
          mCurrentShapeCount++;
          drawShapeContours(mDisplayImage, mCurrentContours.at(i));
          setShapeValues(mDisplayImage, mCurrentContours.at(i));
        }
      }
    }
    break;
  }
  case SHAPES::CIRCLE:
  {
    for (int i = 0; i < mCurrentContours.size(); i++)
    {
      double epsilon = mEpsilonMultiply * arcLength(mCurrentContours.at(i), true);
      approxPolyDP(mCurrentContours.at(i), mApproxImage, epsilon, true);
      if (mApproxImage.size().height > 5)
      {
        if (contourArea(mCurrentContours.at(i)) < mMinContourSize || contourArea(mCurrentContours.at(i)) > mMaxContourSize)
        {
          //Ignore small or huge shapes
        }
        else
        {
          mCurrentShapeCount++;
          drawShapeContours(mDisplayImage, mCurrentContours.at(i));
          setShapeValues(mDisplayImage, mCurrentContours.at(i));
        }
      }
    }
    break;
  }
  case SHAPES::HALFCIRCLE:
  {
    break;
  }
  case SHAPES::UNKNOWNSHAPE:
  {
    std::cout << "ERROR - Unknown shape" << std::endl;
    break;
  }
  }
  return mCurrentContours;
}

void Shapedetector::drawShapeContours(Mat aImage, Mat aContour)
{
  drawContours(aImage, aContour, -1, Scalar(0, 255, 0), 3);
}

void Shapedetector::setShapeValues(Mat aImage, Mat aContour)
=======
void Shapedetector::onChange(int, void *)
{
  // Set blur
  // Set contrast
  // Set noise
}

void Shapedetector::setShapeFound(Mat aImage)
>>>>>>> 80e274eec042b75ab72fac6679db51d53ba49bcd
{
  const std::string shapeCountText = std::to_string(mCurrentShapeCount) + " " + ShapeToString(mCurrentShape);
  printShapeFound();
  putText(aImage, shapeCountText, Point(mTimeXOffset, (mTimeYOffset * 2)), FONT_HERSHEY_SIMPLEX, mTextSize, Scalar(0, 0, 0), 1);
}

void Shapedetector::setTimeValue(Mat aImage, std::clock_t aStartTime, std::clock_t aEndTime)
{
  printTimeValue(aStartTime, aEndTime);
  double calcTime = 1000.0 * ((double)aEndTime - (double)aStartTime) / CLOCKS_PER_SEC;
  const std::string timeText = std::string("T:" + std::to_string(calcTime) + " ms");
  putText(aImage, timeText, Point(mTimeXOffset, mTimeYOffset), FONT_HERSHEY_SIMPLEX, mTextSize, Scalar(0, 0, 0), 1);
}

Mat Shapedetector::removeNoise(Mat aImage)
{
  Mat result;
  Mat structure = getStructuringElement(MORPH_RECT, Size(5, 5));
  morphologyEx(aImage, result, MORPH_OPEN, structure);
  return result;
}