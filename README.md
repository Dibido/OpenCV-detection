# OpenCV ShapeDectector
## Program description
This program works in the following ways:
* Batch input:  
In this mode the program reads its commands from a file and executes them in order.
* Interactive mode:  
In this mode the program gets issued commands from the commandline interface until an exit command is entered.

## Compilation instructions
This program has been created using CMake. Use the following commands to compile:  
``` Bash
mkdir build
cd build
cmake ..
make
```
## Arguments
shapedetector batch.txt
shapedetector
## Commands
### Syntax
[form][whitespace][color][newline]
### Examples
circle red\n
### Comments
\# This is a comment

## Output
### Interactive mode
* Show contours of the form
* X/Y points of the center of the form
* Area of the form
* Time in cycles to find the result (std::clock)
* Whether any shapes were detected
### Batch mode
* Data from interactive mode to STDOUT
## Compilation requirements
* Using the C++-14 standard.
* Compiled with -Wall -Wextra -Wconversion without errors.
* The compiler should show *All* output.
* Codecheckers (Codan / cppcheck) should not show any errors.
* Valgrind should not show any errors when run with all levels turned on.
* Headerfiles contain the necessary Doxygen comments.