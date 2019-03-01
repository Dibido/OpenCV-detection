// Local
#include "Shapedetector.h"

// Constructor
Shapedetector::Shapedetector()
{
}

Shapedetector::Shapedetector(std::string aImageFilePath) : mImagePath(aImageFilePath)
{
    // Store origininal image
    mOriginalImage = imread(mImagePath);
    mOriginalImage.copyTo(mDisplayImage);
    mOriginalImage.copyTo(mTresholdImage);

    // Convert to necessary formats
    cvtColor(mOriginalImage, mGreyImage, CV_BGR2GRAY);
    cvtColor(mOriginalImage, mHSVImage, CV_BGR2HSV);

    initializeValues();
}

void Shapedetector::setImage(Mat aImage)
{
    // Store origininal image
    mOriginalImage = aImage;
    mOriginalImage.copyTo(mDisplayImage);
    mOriginalImage.copyTo(mTresholdImage);

    // Convert to necessary formats
    cvtColor(mOriginalImage, mGreyImage, CV_BGR2GRAY);
    cvtColor(mOriginalImage, mHSVImage, CV_BGR2HSV);

    initializeValues();
}

void Shapedetector::reset()
{
    // Reload frames
    mOriginalImage.copyTo(mDisplayImage);
    mOriginalImage.copyTo(mTresholdImage);
    cvtColor(mOriginalImage, mHSVImage, CV_BGR2HSV);
    mCurrentShapeCount = 0; // Reset shape count
}

void Shapedetector::initializeValues()
{
    // Set the calibration variables
    mBlurSliderValue = 1;
    mContrastSliderValue = 1;
    mNoiseSliderValue = 0;

    mBlurSliderRange = 11;
    mContrastSliderRange = 80;
    mNoiseSliderRange = 10;

    mScreenDrawWidth = 600;
    mScreenDrawHeight = mScreenDrawWidth * 1080 / 1920;

    // Set the blur variables
    mGaussianKernelsize = Size(3, 3);

    // Set the treshold variables
    mMinTreshold = 120;
    mMaxTreshold = 255;
    mTresholdType = ThresholdTypes::THRESH_BINARY;
    mMinSquareRatio = 0.85;
    mMaxSquareRatio = 1.08;

    // Set the Contours variables
    mContourCenterMargin = 30;
    mCurrentShapeCount = 0;
    mEpsilonMultiply = 0.04;
    mMinContourSize = 500.0;
    mMaxContourSize = 30000.0;
    mMaxHalfCircleInlierPercentage = 60.0;
    mMinHalfCircleInlierPercentage = 10.0;
    mMinHalfCirclePercentage = 50.0;
    mMaxHalfCirclePercentage = 72.0;
    mTextOffset = 20;
    mTextSize = 0.5;

    // Set the timer variables
    mTimeXOffset = 20;
    mTimeYOffset = 20;

    // Set the color limits for color detection [0] = Min, [1] = Max
    mBlueLimits[0] = Scalar(95, 55, 20);
    mBlueLimits[1] = Scalar(135, 255, 255);
    // mGreenLimits[0] = Scalar(40, 20, 20);
    // mGreenLimits[1] = Scalar(90, 255, 255);
    mGreenLimits[0] = Scalar(25, 45, 24);
    mGreenLimits[1] = Scalar(90, 255, 130);
    mRedLimits[0] = Scalar(0, 60, 60);
    mRedLimits[1] = Scalar(10, 255, 255);
    mRedLimits[2] = Scalar(170, 60, 60);
    mRedLimits[3] = Scalar(180, 255, 255);
    mBlackLimits[0] = Scalar(0, 0, 0);
    mBlackLimits[1] = Scalar(255, 100, 100);
    mYellowLimits[0] = Scalar(25, 60, 60);
    mYellowLimits[1] = Scalar(45, 255, 255);
    mWhiteLimits[0] = Scalar(10, 30, 20); // Is woodcolor instead of white
    mWhiteLimits[1] = Scalar(24, 255, 255);
}

// Destructor
Shapedetector::~Shapedetector()
{
}

bool Shapedetector::parseSpec(const std::string &aShapeCommand)
{
    bool result = true;

    // Parse command
    std::size_t delimiterPos = aShapeCommand.find(' ');
    std::string shapeStr = aShapeCommand.substr(0, delimiterPos);
    std::string colorStr = aShapeCommand.substr(delimiterPos + 1);

    mCurrentColor = StringToColor(colorStr); // convert string to enum
    if (mCurrentColor == COLORS::UNKNOWNCOLOR)
    {
        std::cout << "Error: unkown color entered" << std::endl;
        result = false;
    }

    mCurrentShape = StringToShape(shapeStr); // convert string to enum
    if (mCurrentShape == SHAPES::UNKNOWNSHAPE)
    {
        std::cout << "Error: unkown shape entered" << std::endl;
        result = false;
    }

    return result;
}

// Shape detectors "main"
void Shapedetector::handleShapeCommand(const std::string &aShapeCommand)
{
    bool parsingSucceeded = parseSpec(aShapeCommand);
    if (parsingSucceeded == false)
    {
        std::cout << "Error: invalid specification entered" << std::endl;
    }

    draw(); // draw windows and sliders (once)

    while (true)
    {
        bool keyPressed = showImages(); // refresh images continiously
        if (keyPressed)
        {
            break;
        }
    }
}

bool Shapedetector::showImages()
{
    bool keyPressed = false;

    reset();     // reset images and values
    recognize(); // run algorithm

    // Show images
    imshow("Original", mOriginalImage);
    imshow("Color", mMaskImage);
    imshow("Result", mDisplayImage);

    // imshow("Brightness", mBrightenedRgbImage);
    // imshow("Blur", mBlurredImage);
    // imshow("Mask", mMaskImage);

    int pressedKey = waitKey(30);
    if (pressedKey == 27) // ESC key
    {
        destroyAllWindows();
        keyPressed = true;
    }

    return keyPressed;
}

// Draws the windows and text
void Shapedetector::draw()
{
    setTimeValue(mDisplayImage, mClockStart, mClockEnd); // draw durations
    setShapeFound(mDisplayImage);                        // draw found shapes

    // Show original
    namedWindow("Original", WINDOW_NORMAL);
    moveWindow("Original", 0, 0);
    resizeWindow("Original", mScreenDrawWidth, mScreenDrawHeight);

    // Show mask (optional)
    namedWindow("Color", WINDOW_NORMAL);
    moveWindow("Color", mScreenDrawWidth, 0);
    resizeWindow("Color", mScreenDrawWidth, mScreenDrawHeight);

    // Show result
    namedWindow("Result", WINDOW_NORMAL);
    moveWindow("Result", mScreenDrawWidth * 2, 0);
    resizeWindow("Result", mScreenDrawWidth, mScreenDrawHeight);

    // resizeWindow("Brightness", mScreenDrawWidth, mScreenDrawHeight);
    // resizeWindow("Blur", mScreenDrawWidth, mScreenDrawHeight);
    // resizeWindow("Color", mScreenDrawWidth, mScreenDrawHeight);

    // Sliders
    namedWindow("Sliders");
    createTrackbar("Brightness", "Sliders", &mContrastSliderValue, mContrastSliderRange, onChange, this);
    createTrackbar("Blur\t\t", "Sliders", &mBlurSliderValue, mBlurSliderRange, onChange, this);
    createTrackbar("Noise\t\t", "Sliders", &mNoiseSliderValue, mNoiseSliderRange, onChange, this);
    moveWindow("Sliders", 0, mOriginalImage.rows + 10);

    const int sliderWidth = 500;
    Mat emptyMatrix = Mat::zeros(1, sliderWidth, CV_8U);
    imshow("Sliders", emptyMatrix); // put an empty matrix in this window to prevent errors
}

void Shapedetector::printDetectionData()
{
    printTimeValue(mClockStart, mClockEnd);
    printShapeFound();

    // int time = 0;
    // std::string nShapes = std::to_string(mCurrentShapeCount);
    // std::string shape = ShapeToString(mCurrentShape);

    // std::cout << "T = " << time << "\t";
    // std::cout << "\"" << shape << "\" count = " << nShapes << "\t";
    // std::cout << "T = " << time << "\t";
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
    mClockStart = std::clock(); // Start timer

    ////////////////////////////////////////
    // Constrain/manipulate slider values
    ////////////////////////////////////////
    if (mNoiseSliderValue == 0)
    {
        mNoiseSliderValue++; // noiseValue must be > 0
    }
    if (mBlurSliderValue % 2 == 0) // blur kernel size must be an odd value
    {
        mBlurSliderValue++;
    }

    //////////////////////
    // Apply filters
    //////////////////////

    // 1. Change brightness
    Mat brightenedBGRImage;
    Mat brightenedHSVImage;
    mOriginalImage.convertTo(brightenedBGRImage, -1, 1, mContrastSliderValue);
    cvtColor(brightenedBGRImage, brightenedHSVImage, COLOR_BGR2HSV);
    mBrightenedRgbImage = brightenedBGRImage;

    // 2. Blur
    Mat blurredHSVImage;
    Size blurValue = Size(mBlurSliderValue, mBlurSliderValue);
    GaussianBlur(brightenedHSVImage, blurredHSVImage, blurValue, 0);
    cvtColor(blurredHSVImage, mBlurredImage, COLOR_HSV2BGR); // save blurred output

    // 3. Filter color
    mMaskImage = detectColor(mCurrentColor, blurredHSVImage);

    // 4. Remove noise
    Mat removedNoise = removeNoise(mMaskImage);

    //////////////////
    // Detect shapes
    //////////////////

    detectShape(mCurrentShape, removedNoise);
    mClockEnd = std::clock(); // Stop timer
}

void Shapedetector::onChange(int, void *)
{
}

void Shapedetector::setShapeFound(Mat aImage)
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
    Mat structure = getStructuringElement(MORPH_RECT, Size(mNoiseSliderValue, mNoiseSliderValue));
    // Mat structure = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(aImage, result, MORPH_OPEN, structure);
    return result;
}

void Shapedetector::startCommandline(std::string imgPath)
{
    std::cout << "### Interactive mode ###" << std::endl;

    while (true)
    {
        std::cout << "Please enter [vorm] [kleur]" << std::endl;
        std::cout << "> ";
        std::string command;
        getline(std::cin, command); // Get command

        if (command != EXIT_COMMAND)
        {
            Mat img = imread(imgPath);
            setImage(img);
            handleShapeCommand(command); // Start algorithm
        }
        else if (command == EXIT_COMMAND)
        {
            std::cout << "Closing program.." << std::endl;
            break;
        }
        else
        {
            break;
        }
    }
}

void Shapedetector::webcamMode(int deviceId)
{
    // Set pretake cam framecount
    const int WEBCAM_START_IMAGES = 10;

    // Read image from webcam
    VideoCapture cap;
    Mat capturedImage;
    cap.open(deviceId);
    if (cap.isOpened())
    {
        for (size_t i = 0; i < WEBCAM_START_IMAGES; i++)
        {
            cap.grab();
            cap.retrieve(capturedImage);
        }
        cap.retrieve(capturedImage);
        cap.release();
        imwrite("webcamblocks1.png", capturedImage);
    }

    // Start webcam mode
    std::cout << "### Webcam mode ###" << std::endl;
    std::cout << "Please enter [vorm] [kleur]" << std::endl;

    setImage(capturedImage); // create shape detector

    while (true)
    {
        std::cout << "> ";
        std::string command;
        getline(std::cin, command); // Get command

        if (command != EXIT_COMMAND)
        {
            handleShapeCommand(command); // Start algorithm
        }
        else if (command == EXIT_COMMAND)
        {
            std::cout << "Closing program.." << std::endl;
            break;
        }
        else
        {
            break;
        }
    }
}

void Shapedetector::batchMode(int cameraId, std::string batchPath)
{
    initCamera(cameraId);

    if (fileExists(batchPath) == false)
    {
        std::cout << "Error: batch file does not exist (" << batchPath << ")" << std::endl;
    }
    else
    {
        std::cout << "### Batch mode ###" << std::endl;

        std::string line;
        std::ifstream batchFile(batchPath);

        while (std::getline(batchFile, line)) // for every line in the file
        {
            if (line.at(0) != COMMENT_CHARACTER) // if line doesnt start with comment char
            {
                std::cout << "Detecting \"" << line << "\".." << std::endl;

                bool parsingSucceeded = parseSpec(line);
                if (parsingSucceeded == false)
                {
                    std::cout << "Error: invalid specification entered" << std::endl;
                }

                Mat retrievedFrame;
                mVidCap.grab();
                mVidCap.retrieve(retrievedFrame);
                setImage(retrievedFrame);

                draw();

                while (true)
                {
                    bool keyPressed = showImages();
                    if (keyPressed)
                    {
                        break;
                    }
                }
            }
        }
    }
}

void Shapedetector::initCamera(int cameraId)
{
    mVidCap.open(cameraId);

    if (mVidCap.isOpened() == false)
    {
        std::cout << "Error: video capture not opened" << std::endl;
    }
}