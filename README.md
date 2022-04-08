# SoundCath Real Time UltraSound Renderer

## Brief

This Project is a real time renderer for Oldelft Ultrasound Boxes and Catheters.

## Authors

- [Orion Serup](orionserup@gmail.com)

## Dependencies

- A C++ Compiler (MSVC, GCC, or Clang)

- [Eigen](https://gitlab.com/libeigen/eigen.git) For the Linear Algebra

- [VTK](https://gitlab.kitware.com/vtk/vtk) For the Rendering and Visualization
- [CMake](https://cmake.org/) For building and Installing

## Optional Dependencies

- [Catch2](https://github.com/catchorg/Catch2) For Testing and Verification

- [Doxygen](https://www.doxygen.nl) For Documentation Generation

## Build Guide
In order to build this project you need to use CMake to compile and generate build files. Either using _cmake-gui_, _ccmake_ or cli _cmake_ navigate to the build directory and from there configure and generate to that folder. Using _ccmake_ or _cmake-gui_ that will be just pressing those buttons and setting the output to the same folder. For the cli _cmake_ that will be:

__cmake .__

Then to build the project either use the open folder option in cmake-gui or quit in ccmake. The command to build the file is

__cmake --build .__

For Windows using MSVC, you want to build the ALL_BUILD Target in Visual Studio after clicking the open folder in cmake-gui
## Installation Guide
   - For MSVC/Visual Studio Build the INSTALL Target
   - For Linux/Mac run "sudo cmake --install ."
## Outputs and Usage 
### Outputs
- Static Library in the lib folder for external utility 
- Executable in the bin folder that runs the renderer
- Test Executable in the test folder if generating tests
- Documentation in the docs folder if generating the docs (may have to use Doxywizard to generate in a windows system)
### Testing
Testing is operated through Catch2. An Executable is generated in the test folder, running it will spit the results to a log file as well as to the standard output.

### Usage
Running the program is very simple, just run the executable in the bin folder. This can be linked to anywhere in the system, it is important that the executable itself is not moved due to the dependence on a relatively located dll.
