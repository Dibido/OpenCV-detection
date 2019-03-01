# OpenCV ShapeDectector
## Program description
This program works in the following ways:
* Batch input:  
In this mode the program reads its commands from a file and executes them in order.
* Interactive mode:  
In this mode the program gets issued commands from the commandline interface until an exit command is entered.

## Software design
The functions prototypes for the filters are as follows:  
```
Mat Foo(const Mat& aMat)
{
  Mat result;
  result.change();
  return result;
}
```

## Compilation instructions
This program has been created using CMake. Use the following commands to compile:  
``` Bash
mkdir build
cd build
cmake ..
make
```
And to run in the different modes:
``` Bash
./shapedetector 1 #Webcam mode
./shapedetector 1 ../example_batch.txt #Batch mode
```
## Arguments
Batch:  
``` Bash
shapedetector [cameraId] [batchfile]
```
Interactive:  
``` Bash
shapedetector [cameraId]
```
## Commands
### Syntax
``` Bash
[form][whitespace][color][newline]
```
### Examples
``` Bash
cirkel rood\n
```
### Comments
``` Bash
\# This is a comment
```

## Output
### Interactive mode
* Show contours of the form
* X/Y points of the center of the form
* Area of the form in pixels
* Time in cycles to find the result (std::clock)
* Whether any shapes were detected (the number of found objects)
### Batch mode
* Data from interactive mode to STDOUT
## Compilation requirements
* Using the C++-14 standard.
* Compiled with -Wall -Wextra -Wconversion without errors.
* The compiler should show *All* output.
* Codecheckers (Codan / cppcheck) should not show any warnings.
* Valgrind should be able to run flawlessly when run with all levels turned on.
* Headerfiles contain the necessary Doxygen comments.