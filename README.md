# SoundCath Real Time UltraSound Renderer

## Brief

This Project is a real time renderer for Oldelft Ultrasound Boxes and Catheters.

## Authors

### Orion Serup


## Dependencies

### [Eigen](https://gitlab.com/libeigen/eigen.git) For the Linear Algebra

### [VTK](https://gitlab.kitware.com/vtk/vtk) For the Rendering and Visualization

### [Doxygen](https://www.doxygen.nl) For Documentation Generation


## Build Guide
In order to build this project you need to use CMake to compile and generate build files. Either using cmake-gui, ccmake or cli cmake navigate to the build directory and from there configure and generate to that folder. Using ccmake or cmake-gui that will be just pressing those buttons and setting the output to the same folder. For the cli cmake that will be:

cmake . -DCMAKE_BUILD_TYPE=Release

Then to build the project either use the open folder option in cmake-gui or quit in ccmake. The command to build the file is

cmake --build .