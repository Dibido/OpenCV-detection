#include "Shapedetector.h"

Mat Shapedetector::detectColor(COLORS aColor, Mat aImage)
{
  Mat resultMask;
  Mat tempMask;
  Mat resultImage;
  switch (aColor)
  {
    case COLORS::BLUE:
    {
      inRange(aImage, mBlueLimits[0], mBlueLimits[1], resultMask);
      break;
    }
    case COLORS::GREEN:
    {
      inRange(aImage, mGreenLimits[0], mGreenLimits[1], resultMask);
      break;
    }
    case COLORS::RED:
    {
      inRange(aImage, mRedLimits[0], mRedLimits[1], resultMask);
      inRange(aImage, mRedLimits[2], mRedLimits[3], tempMask);
      bitwise_or(resultMask, tempMask, resultMask);
      break;
    }
    case COLORS::BLACK:
    {
      inRange(aImage, mBlackLimits[0], mBlackLimits[1], resultMask);
      break;
    }
    case COLORS::YELLOW:
    {
      inRange(aImage, mYellowLimits[0], mYellowLimits[1], resultMask);
      break;
    }
    case COLORS::WHITE:
    {
      inRange(aImage, mWhiteLimits[0], mWhiteLimits[1], resultMask);
      break;
    }
    case COLORS::UNKNOWNCOLOR:
    {
      std::cout << "Error: unknown color = " << aColor << std::endl;
      break;
    }
  }
  return resultMask;
}