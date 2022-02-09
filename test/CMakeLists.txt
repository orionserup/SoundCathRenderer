cmake_minimum_required(VERSION 3.10)

include(CTest)

include_directories(../include)

file(GLOB ASICTESTS ./ASIC/*.cpp)
file(GLOB FPGATESTS ./FPGA/*.cpp)
file(GLOB DRIVERTESTS ./Driver/*.cpp)
file(GLOB CONTROLTESTS ./Controller/*.cpp)
file(GLOB RENDERTESTS ./Renderer/*.cpp)

add_executable(ASICTester ${ASICTESTS})
add_executable(FPGATester ${FPGATESTS})
add_executable(DriverTester ${DRIVERTESTS})
add_executable(ControllerTester ${CONTROLTESTS})
add_executable(RendererTester ${RENDERTESTS})

# All of the Tests to Run
add_test(NAME DRIVERQUERY COMMAND ./DriverTester --query)