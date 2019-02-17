#include "Shapedetector.h"

Shapedetector::Shapedetector(std::string aImageFilePath) :
    mImagePath(aImageFilePath)
{
  mOriginalImage = imread(mImagePath);
}

Shapedetector::~Shapedetector()
{
}

void Shapedetector::handleShapeCommand(const std::string& aShapeCommand)
{
  //Parse command
  std::size_t delimiterPos = aShapeCommand.find(' ');
  std::string commandShape = aShapeCommand.substr(0, delimiterPos);
  std::string commandColor = aShapeCommand.substr(delimiterPos + 1);

  mCurrentColor = StringToColor(commandColor);
  mCurrentShape = StringToShape(commandShape);

  detectColor(mCurrentColor);
  detectShape(mCurrentShape);

  std::cout << aShapeCommand << std::endl;
  imshow("oringinal", mOriginalImage);
  waitKey(0);
}

int Shapedetector::detectColor(COLORS aColor)
{
  //TODO: Implement
}

int Shapedetector::detectShape(SHAPES aShape)
{
  //TODO: Implement
}