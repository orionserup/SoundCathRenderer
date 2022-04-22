cmake_minimum_required( VERSION 3.10 )

project(SoundCathRenderer 
        LANGUAGES CXX 
        VERSION 0.1
        DESCRIPTION "A Real Time UltraSound Rendering and Visualization Platform")

# --------------------- Dependencies ------------------ #

find_package(Eigen3 3.4 REQUIRED NO_MODULE)

find_package(VTK COMPONENTS
    CommonColor
    CommonCore
    CommonDataModel
    FiltersCore
    FiltersSources
    IOXML
    ImagingHybrid
    InteractionStyle
    RenderingContextOpenGL2
    RenderingCore
    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
)
if(NOT VTK_FOUND)
    add_subdirectory(../3rdparty/vtk ../3rdparty/vtk/build)
endif()

find_package(gcem)
if(NOT gcem_FOUND)
    add_subdirectory(../3rdparty/gcem ../3rdparty/gcem/build)
endif()

find_package(fmt)
if(NOT fmt_FOUND)
    add_subdirectory(../3rdparty/fmt ../3rdparty/fmt/build)
endif()

find_package(plog)
if(NOT plog_FOUND)
    add_subdirectory(../3rdparty/plog ../3rdparty/plog/build)
endif()

#---------------------- Compile Options ----------------- #

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)

    set(CMAKE_CXX_FLAGS "/W4 /Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "/DDEBUG /Od /D_DEBUG /DEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "/DNDEBUG /D_NDEBUG /LTCG /Ot /GL")

else()

    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -Og -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3 -march=native -mtune=native")

endif()

file(GLOB SOURCES "../src/*[!main].cpp" "../src/Exception.cpp")
file(GLOB INCLUDES "../include/*.h*")

file(GLOB TESTFILES "../test/*/*.hpp" "../test/*/*.cpp")
set(DOCSOURCES "../src" "../include" "../test/" "../README.md")

#------------------- Compile Targets --------------------- #

add_library(UltraSound STATIC ${SOURCES})
target_include_directories(UltraSound PUBLIC ../include)

set_target_properties(UltraSound PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY ../lib
                LIBRARY_OUTPUT_DIRECTORY ../lib)

vtk_module_autoinit(TARGETS UltraSound
                    MODULES ${VTK_LIBRARIES})

add_executable(${PROJECT_NAME} ../src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ../include ${VTK_USE_FILE})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../bin)   

target_link_libraries(UltraSound ${VTK_LIBRARIES}) 
target_link_libraries(UltraSound Eigen3::Eigen) 
target_link_libraries(UltraSound fmt::fmt)
target_link_libraries(UltraSound gcem)
target_link_libraries(UltraSound plog::plog)

target_link_libraries(${PROJECT_NAME} UltraSound)

include(GNUInstallDirs)
install(TARGETS UltraSound
        CONFIGURATIONS Release
        DESTINATION ${CMAKE_INSTALL_LIBRARY_PATH})

install(FILES ${INCLUDES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SoundCath)

# ------------- Documentation Generation ------------------ #

option(BUILD_DOCS "Generate HTML and PDF Referance Manuals (Requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCS)

    find_package(Doxygen REQUIRED dot)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "You Need Doxygen To Build the Docs")
    endif()
    
    set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
    set(DOXYGEN_PROJECT_BRIEF ${PROJECT_DESCRIPTION})
    
    set(DOXYGEN_OUTPUT_DIRECTORY ../docs)

    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX YES)

    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_FULL_PATH_NAMES NO)
    set(DOXYGEN_BUILTIN_STL_SUPPORT YES)

    set(DOXYGEN_SOURCE_BROWSER YES)

    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ../README.md)

    doxygen_add_docs(docs
        ${DOCSOURCES}
        WORKING_DIRECTORY ../docs
        COMMENT "Generating PDF and HTML Documentation")

endif()

# -------------------- Testing ----------------------- #

option(ENABLE_TESTING "Create A Target That Tests the Software" OFF)
if(ENABLE_TESTING)
    add_subdirectory(../test ../test)
endif()

