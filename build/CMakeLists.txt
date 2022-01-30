cmake_minimum_required( VERSION 3.10 )

project(SoundCathRenderer LANGUAGES CXX VERSION 0.1)

find_package(Eigen3 3.4 REQUIRED NO_MODULE)

find_package(VTK COMPONENTS
    vtkCommonColor
    vtkCommonCore
    vtkCommonDataModel
    vtkFiltersCore
    vtkFiltersSources
    vtkIOXML
    vtkImagingHybrid
    vtkInteractionStyle
    vtkRenderingContextOpenGL2
    vtkRenderingCore
    vtkRenderingFreeType
    vtkRenderingGL2PSOpenGL2
    vtkRenderingOpenGL2
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)

set(CMAKE_CXX_FLAGS "/W4 /Wall")
set(CMAKE_CXX_FLAGS_DEBUG "/DDEBUG /Od /D_DEBUG /DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "/LTCG /Ot /GL")

else()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -Og -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -Ofast -march=native -mtune=native")

set(CMAKE_EXE_LINKER_FLAGS "-flto")

endif()

file(GLOB SOURCES ../src/*[!main].cpp)
add_library(UltraSound STATIC ${SOURCES})
target_include_directories(UltraSound PUBLIC ../include )

set_target_properties(UltraSound PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY ../lib
                LIBRARY_OUTPUT_DIRECTORY ../lib)

vtk_module_autoinit(TARGETS UltraSound
                    MODULES ${VTK_LIBRARIES})

add_executable(${PROJECT_NAME} ../src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../bin)   

target_link_libraries(UltraSound ${VTK_LIBRARIES} Eigen3::Eigen)
target_link_libraries(${PROJECT_NAME} UltraSound)

install(TARGETS UltraSound DESTINATION ${CMAKE_INSTALL_LIBRARY_PATH})
