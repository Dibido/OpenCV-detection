// Local
#include "Shapedetector.h"

Shapedetector::Shapedetector(std::string aImageFilePath) : mImagePath(aImageFilePath)
{
  // Store origininal image
  mOriginalImage = imread(mImagePath);
  mOriginalImage.copyTo(mDisplayImage);
  mOriginalImage.copyTo(mTresholdImage);
  
  // Convert to necessary formats
  cvtColor(mOriginalImage, mGreyImage, CV_BGR2GRAY);
  cvtColor(mOriginalImage, mHSVImage, CV_BGR2HSV);

  // Set the blur variables
  mGaussianKernelsize = Size(5, 5);

  // Set the treshold variables
  mMinTreshold = 120;
  mMaxTreshold = 255;
  mTresholdType = ThresholdTypes::THRESH_BINARY;

  // Set the Contours variables
  mMinContourSize = 500.0;
  mMaxContourSize = 30000.0;
  mTextOffset = 20;
  mTextSize = 0.5;

  // Set the timer variables
  mTimeXOffset = 20;
  mTimeYOffset = 20;

  // Set the color limits for color detection [0] = Min, [1] = Max
  mBlueLimits[0] = Scalar(85, 40, 30);
  mBlueLimits[1] = Scalar(135, 255, 255);
  mGreenLimits[0] = Scalar(40, 25, 25);
  mGreenLimits[1] = Scalar(80, 255, 255);
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
  // TODO: find the other limits
}

Shapedetector::~Shapedetector()
{
}

void Shapedetector::handleShapeCommand(const std::string &aShapeCommand)
{
  // Show command
  std::cout << aShapeCommand << std::endl;
  // Parse command
  std::size_t delimiterPos = aShapeCommand.find(' ');
  std::string commandShape = aShapeCommand.substr(0, delimiterPos);
  std::string commandColor = aShapeCommand.substr(delimiterPos + 1);

  mCurrentColor = StringToColor(commandColor);
  mCurrentShape = StringToShape(commandShape);

  //Reset values
  mOriginalImage.copyTo(mDisplayImage);
  mOriginalImage.copyTo(mTresholdImage);

  // Start timer
  mClockStart = std::clock();

  mMaskImage = detectColor(mCurrentColor);
  detectShape(mCurrentShape);

  // Stop timer
  mClockEnd = std::clock();
  // Put timer value in image
  setTimeValue(mDisplayImage, mClockStart, mClockEnd);

  //Show results
  imshow("result", mDisplayImage);
  moveWindow("result", 0, 0);
  imshow("mask", mMaskImage);
  moveWindow("mask", mOriginalImage.cols, 0);

  waitKey(0);

	// int r = 50;
	// int range = 100;
	// createTrackbar("R", "mask", &r, range);

	// int pressedKey = waitKey(0);
	// if (pressedKey = ESC_KEY) { /* do something */ }

	/*
	PSEUDO CODE:

	set color: red, green, blue, yellow, black, white
	set shape: circle, halfCircle, square, rect, triangle
	*/
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
      std::cout << "ERROR - Unknown color" << std::endl;
      break;
    }
	}
	resultMask.copyTo(mCurrentMask);
	mOriginalImage.copyTo(resultImage, resultMask);
	return resultImage;
}

std::vector<Mat> Shapedetector::detectShape(SHAPES aShape)
{
  GaussianBlur(mCurrentMask, mCurrentMask, mGaussianKernelsize, BorderTypes::BORDER_DEFAULT);
  // GaussianBlur(mGreyImage, mGreyImage, mGaussianKernelsize, BorderTypes::BORDER_DEFAULT);
  adaptiveThreshold(mCurrentMask, mTresholdImage, 255, AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C, mTresholdType, 5, 2);
  findContours(mTresholdImage, mCurrentContours, CV_RETR_TREE, CHAIN_APPROX_NONE);
  switch (aShape)
  {
  case SHAPES::ALL:
  {
    for (int i = 0; i < mCurrentContours.size(); i++)
    {
      double epsilon = 0.04 * arcLength(mCurrentContours.at(i), true);
      approxPolyDP(mCurrentContours.at(i), mApproxImage, epsilon, true);
      mCurrentShapeCount++;
      drawShapeContours(mDisplayImage, mCurrentContours.at(i));
      setShapeValues(mDisplayImage, mCurrentContours.at(i));
    }
    break;
  }
  case SHAPES::SQUARE:
  {
    for (int i = 0; i < mCurrentContours.size(); i++)
    {
      double epsilon = 0.04 * arcLength(mCurrentContours.at(i), true);
      approxPolyDP(mCurrentContours.at(i), mApproxImage, epsilon, true);
      if (mApproxImage.size().height == 4)
      {
        mCurrentShapeCount++;
        drawShapeContours(mDisplayImage, mCurrentContours.at(i));
        setShapeValues(mDisplayImage, mCurrentContours.at(i));
      }
    }
    break;
  }
  case SHAPES::RECTANGLE:
  {
    for (int i = 0; i < mCurrentContours.size(); i++)
    {
      double epsilon = 0.04 * arcLength(mCurrentContours.at(i), true);
      approxPolyDP(mCurrentContours.at(i), mApproxImage, epsilon, true);
      if (mApproxImage.size().height == 4)
      {
        drawShapeContours(mDisplayImage, mCurrentContours.at(i));
        setShapeValues(mDisplayImage, mCurrentContours.at(i));
      }
    }
    break;
  }
  case SHAPES::TRIANGLE:
  {
    for (int i = 0; i < mCurrentContours.size(); i++)
    {
      double epsilon = 0.04 * arcLength(mCurrentContours.at(i), true);
      approxPolyDP(mCurrentContours.at(i), mApproxImage, epsilon, true);
      if (mApproxImage.size().height == 3)
      {
        drawShapeContours(mDisplayImage, mCurrentContours.at(i));
        setShapeValues(mDisplayImage, mCurrentContours.at(i));
      }
    }
    break;
  }
  case SHAPES::CIRCLE:
  {
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
  if(contourArea(aContour) < mMinContourSize || contourArea(aContour) > mMaxContourSize)
  {
    //Ignore small or huge shapes
  }
  else
  {
  drawContours(aImage, aContour, -1, Scalar(0, 255, 0), 3);
  }
}

void Shapedetector::setShapeValues(Mat aImage, Mat aContour)
{
  Moments currentmoments;
  if( contourArea(aContour) < mMinContourSize || contourArea(aContour) > mMaxContourSize)
  {
    //Ignore small or huge shapes
  }
  else
  {
      currentmoments = moments(aContour);
      int cX = (int)(currentmoments.m10 / currentmoments.m00);
      int cY = (int)(currentmoments.m01 / currentmoments.m00);
      const std::string xPosString = std::string("X:" + std::to_string(cX));
      const std::string yPosString = std::string("Y:" + std::to_string(cY));
      const std::string areaString = std::string("A:" + std::to_string((int)contourArea(aContour)));
      putText(aImage, xPosString, Point(cX, cY), FONT_HERSHEY_SIMPLEX, mTextSize, Scalar(255, 255, 255), 1);
      putText(aImage, yPosString, Point(cX, cY + mTextOffset), FONT_HERSHEY_SIMPLEX, mTextSize, Scalar(255, 255, 255), 1);
      putText(aImage, areaString, Point(cX, cY + (mTextOffset * 2)), FONT_HERSHEY_SIMPLEX, mTextSize, Scalar(255 , 255, 255), 1);
  }
}

void Shapedetector::setTimeValue(Mat aImage, std::clock_t aStartTime, std::clock_t aEndTime)
{
  std::cout << std::fixed << std::setprecision(2) << "CPU time used: " << 1000.0 * ((double)aEndTime - (double)aStartTime) / CLOCKS_PER_SEC << " ms\n" << std::endl;
  double calcTime = 1000.0 * ((double)aEndTime - (double)aStartTime) / CLOCKS_PER_SEC;
  const std::string timeText = std::string("T:" + std::to_string(calcTime) + "ms");
  putText(aImage, timeText, Point(0 + mTimeXOffset, 0 + mTimeYOffset), FONT_HERSHEY_SIMPLEX, mTextSize, Scalar(0 ,255, 0), 1);
}