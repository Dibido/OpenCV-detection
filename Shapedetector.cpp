#include "Shapedetector.h"

Shapedetector::Shapedetector(std::string aImageFilePath) :
    mImagePath(aImageFilePath)
{
  // Store origininal image
  mOriginalImage = imread(mImagePath);
  // Convert to necessary formats
  cvtColor(mOriginalImage, mGreyImage, CV_BGR2GRAY);
  cvtColor(mOriginalImage, mHSVImage, CV_BGR2HSV);

  // Set the color limits for color detection [0] = Min, [1] = Max
  mBlueLimits[0] = Scalar(85, 60, 40);
  mBlueLimits[1] = Scalar(135, 255, 255);
  mGreenLimits[0] = Scalar(40, 40, 40);
  mGreenLimits[1] = Scalar(75, 255 ,255);
  mRedLimits[0] = Scalar(0, 60, 60);
  mRedLimits[1] = Scalar(10, 255, 255);
  mRedLimits[2] = Scalar(170, 60, 60);
  mRedLimits[3] = Scalar(180, 255, 255);
  // TODO: find the other limits
}

Shapedetector::~Shapedetector()
{
}

void Shapedetector::handleShapeCommand(const std::string& aShapeCommand)
{
  // Show command
  std::cout << aShapeCommand << std::endl;
  // Parse command
  std::size_t delimiterPos = aShapeCommand.find(' ');
  std::string commandShape = aShapeCommand.substr(0, delimiterPos);
  std::string commandColor = aShapeCommand.substr(delimiterPos + 1);

  mCurrentColor = StringToColor(commandColor);
  mCurrentShape = StringToShape(commandShape);

  mMaskImage = detectColor(mCurrentColor);
  detectShape(mCurrentShape);

  
  imshow("original", mOriginalImage);
  imshow("mask", mMaskImage);
  waitKey(0);
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
      break;
    }
    case COLORS::YELLOW:
    {
      break;
    }
    case COLORS::WHITE:
    {
      break;
    }
    case COLORS::UNKNOWNCOLOR:
    {
      std::cout << "ERROR - Unknown color" << std::endl;
      break;
    }
  }
  mOriginalImage.copyTo(resultImage, resultMask);
  return resultImage;
}

int Shapedetector::detectShape(SHAPES aShape)
{
  //TODO: Implement
}