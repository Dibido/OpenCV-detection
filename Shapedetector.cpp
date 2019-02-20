// Local
#include "Shapedetector.h"

Shapedetector::Shapedetector(std::string aImageFilePath) : mImagePath(aImageFilePath)
{
	// Store origininal image
	mOriginalImage = imread(mImagePath);
	mOriginalImage.copyTo(mTresholdImage);
	// Convert to necessary formats
	cvtColor(mOriginalImage, mGreyImage, CV_BGR2GRAY);
	cvtColor(mOriginalImage, mHSVImage, CV_BGR2HSV);

	// Set the blur variables

	// Set the treshold variables
	mMinTreshold = 120;
	mMaxTreshold = 255;
	mTresholdType = ThresholdTypes::THRESH_BINARY;

	// Set the Contours variables
	mMinContourSize = 40;
	mTextOffset = 20;
	mTextSize = 0.5;

	// Set the color limits for color detection [0] = Min, [1] = Max
	mBlueLimits[0] = Scalar(85, 40, 30);
	mBlueLimits[1] = Scalar(135, 255, 255);
	mGreenLimits[0] = Scalar(40, 40, 40);
	mGreenLimits[1] = Scalar(75, 255, 255);
	mRedLimits[0] = Scalar(0, 60, 60);
	mRedLimits[1] = Scalar(10, 255, 255);
	mRedLimits[2] = Scalar(170, 60, 60);
	mRedLimits[3] = Scalar(180, 255, 255);
	mBlackLimits[0] = Scalar(0, 0, 0);
	mBlackLimits[1] = Scalar(255, 100, 100);
	mYellowLimits[0] = Scalar();
	mYellowLimits[1] = Scalar();
	mWhiteLimits[0] = Scalar(); // Is woodcolor instead of white
	mWhiteLimits[1] = Scalar();
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

	mMaskImage = detectColor(mCurrentColor);
	detectShape(mCurrentShape);

	imshow("original", mOriginalImage);
	moveWindow("original", 0, 0);

	imshow("mask", mMaskImage);
	moveWindow("mask", mOriginalImage.cols, 0);

	int r = 50;
	int range = 100;
	createTrackbar("R", "mask", &r, range);

	int pressedKey = waitKey(0);
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
	resultMask.copyTo(mCurrentMask);
	mOriginalImage.copyTo(resultImage, resultMask);
	return resultImage;
}

std::vector<Mat> Shapedetector::detectShape(SHAPES aShape)
{
	// threshold(mGreyImage, mTresholdImage, mMinTreshold, mMaxTreshold, mTresholdType);
	GaussianBlur(mGreyImage, mGreyImage, Size(5, 5), BorderTypes::BORDER_REFLECT101);
	adaptiveThreshold(mGreyImage, mTresholdImage, 255, AdaptiveThresholdTypes::ADAPTIVE_THRESH_GAUSSIAN_C, mTresholdType, 21, 9);
	findContours(mTresholdImage, mCurrentContours, CV_RETR_LIST, CHAIN_APPROX_NONE);
	// for (int i = 0; i < mCurrentContours.size(); i++)
	//   {
	//     double epsilon = 0.04 * arcLength(mCurrentContours.at(i), true);
	//     approxPolyDP(mCurrentContours.at(i), mApproxImage, epsilon, true);
	//       drawContours(mOriginalImage, mCurrentContours.at(i), -1, Scalar(0, 255, 0), 3);
	//   }
	switch (aShape)
	{
	case SHAPES::SQUARE:
	{
		for (int i = 0; i < mCurrentContours.size(); i++)
		{
			double epsilon = 0.04 * arcLength(mCurrentContours.at(i), true);
			approxPolyDP(mCurrentContours.at(i), mApproxImage, epsilon, true);
			if (mApproxImage.size().height == 4)
			{
				drawContours(mOriginalImage, mCurrentContours.at(i), -1, Scalar(0, 255, 0), 3);
				setShapeValues(mCurrentContours.at(i));
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
				drawContours(mOriginalImage, mCurrentContours.at(i), -1, Scalar(0, 255, 0), 3);
				setShapeValues(mCurrentContours.at(i));
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
				drawContours(mOriginalImage, mCurrentContours.at(i), -1, Scalar(0, 255, 0), 3);
				setShapeValues(mCurrentContours.at(i));
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
	}
	case SHAPES::UNKNOWNSHAPE:
	{
		std::cout << "ERROR - Unknown shape" << std::endl;
		break;
	}
	}
	return mCurrentContours;
}

void Shapedetector::setShapeValues(Mat aContour)
{
	Moments currentmoments;
	if (contourArea(aContour) < mMinContourSize)
	{
		//Ignore small shapes
	}
	else
	{
		currentmoments = moments(aContour);
		int cX = (int)(currentmoments.m10 / currentmoments.m00);
		int cY = (int)(currentmoments.m01 / currentmoments.m00);
		const std::string xPosString = std::string("X:" + std::to_string(cX));
		const std::string yPosString = std::string("Y:" + std::to_string(cY));
		const std::string areaString = std::string("A:" + std::to_string((int)contourArea(aContour)));
		putText(mOriginalImage, xPosString, Point(cX, cY), FONT_HERSHEY_SIMPLEX, mTextSize, Scalar(255, 255, 255), 1);
		putText(mOriginalImage, yPosString, Point(cX, cY - mTextOffset), FONT_HERSHEY_SIMPLEX, mTextSize, Scalar(255, 255, 255), 1);
		putText(mOriginalImage, areaString, Point(cX, cY - (mTextOffset * 2)), FONT_HERSHEY_SIMPLEX, mTextSize, Scalar(255, 255, 255), 1);
	}
}