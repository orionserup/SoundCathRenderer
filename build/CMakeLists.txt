<<<<<<< HEAD
cmake_minimum_required( VERSION 3.10 )

project(SoundCathRenderer LANGUAGES CXX VERSION 0.1)

find_package(Eigen3 3.4 REQUIRED NO_MODULE)
find_package(VTK REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)

set(CMAKE_CXX_FLAGS "/W4")
set(CMAKE_CXX_FLAGS_DEBUG "/DDEBUG /D_DEBUG /DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE )

set(CMAKE_EXE_LINKER_FLAGS "-flto")

else()

set(CMAKE_CXX_FLAGS "-Wall -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -Og -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -Ofast -march=native -mtune=native")

set(CMAKE_EXE_LINKER_FLAGS "-flto")

endif()

file(GLOB SOURCES ../src/*[!main].cpp)
add_library(UltraSound ${SOURCES})
target_include_directories(UltraSound PUBLIC 
                    ../include ${VTK_USE_FILE})

set_target_properties(UltraSound PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY ../lib
                LIBRARY_OUTPUT_DIRECTORY ../lib)

add_executable(${PROJECT_NAME} ../src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)

target_link_libraries(UltraSound ${VTK_LIBRARIES} Eigen3::Eigen)
target_link_libraries(${PROJECT_NAME} UltraSound)

=======
cmake_minimum_required( VERSION 3.10)

project(SoundCathRenderer LANGUAGES CXX  VERSION 0.1)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_COMPILER msvc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BINARY_DIR ../bin)

file( GLOB SOURCES ../src/*.cpp )
target_include_directories( "${PROJECT_NAME}" ../include)
target_link_libraries( "${PROJECT_NAME}" ../lib/asic_call_wrapper.dll ${OPENCV_LIBS})
add_executable("${PROJECT_NAME}" SOURCES)
>>>>>>> f7ce3db (Add project files.)






