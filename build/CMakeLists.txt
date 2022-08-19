cmake_minimum_required( VERSION 3.10 )

project(SoundCathRenderer 
        LANGUAGES CXX 
        VERSION 0.1
        DESCRIPTION "A Real Time UltraSound Rendering and Visualization Platform")

# --------------------- Dependencies ------------------ #

find_package(VTK COMPONENTS
    CommonColor
    CommonCore
    FiltersCore
    FiltersSources
    RenderingCore
)

if(NOT VTK_FOUND)
    add_subdirectory(${CMAKE_BINARY_DIR}/../3rdparty/vtk ../3rdparty/vtk/build)
endif()

find_package(gcem)
if(NOT gcem_FOUND)
    add_subdirectory(${CMAKE_BINARY_DIR}/../3rdparty/gcem ../3rdparty/gcem/build)
endif()

find_package(fmt)
if(NOT fmt_FOUND)
    add_subdirectory(${CMAKE_BINARY_DIR}/../3rdparty/fmt ../3rdparty/fmt/build)
endif()

find_package(plog)
if(NOT plog_FOUND)
    add_subdirectory(${CMAKE_BINARY_DIR}/../3rdparty/plog ../3rdparty/plog/build)
endif()


#------------------- Compile Targets --------------------- #

file(GLOB SOURCES "../src/*[!main].cpp")
file(GLOB INCLUDES "../include/*.h*")

file(GLOB TESTFILES "../test/*/*.hpp" "../test/*/*.cpp")
set(DOCSOURCES "../src" "../include" "../test/" "../README.md")

add_library(UltraSound STATIC ${SOURCES})
target_include_directories(UltraSound PRIVATE ../include)

set_target_properties(UltraSound PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY ../lib
                LIBRARY_OUTPUT_DIRECTORY ../lib)

vtk_module_autoinit(TARGETS UltraSound
                    MODULES ${VTK_LIBRARIES})

add_executable(${PROJECT_NAME} ../src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ../include ${VTK_USE_FILE})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../bin)   

target_link_libraries(UltraSound ${VTK_LIBRARIES}) 
target_link_libraries(UltraSound fmt::fmt)
target_link_libraries(UltraSound gcem)
target_link_libraries(UltraSound plog::plog)

target_link_libraries(${PROJECT_NAME} UltraSound)

include(GNUInstallDirs)
install(TARGETS UltraSound
        CONFIGURATIONS Release
        DESTINATION ${CMAKE_INSTALL_LIBRARY_PATH})

install(FILES ${INCLUDES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/SoundCath)

#---------------------- Compile Options ----------------- #

target_compile_features(UltraSound PUBLIC cxx_std_20) # use c++20 and above

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    target_compile_options(UltraSound PUBLIC $<$<CONFIG:RELEASE>:/W4 /Wall /constexpr /constexpr:steps1000000000 /Zm2000>)
    target_compile_options(UltraSound PUBLIC $<$<CONFIG:DEBUG>:/Od /Zi /MDd>)
    target_compile_options(UltraSound PUBLIC $<$<CONFIG:RELEASE>:/Ot /GL>)
    target_compile_definitions(UltraSound PUBLIC $<$<CONFIG:DEBUG>: _DEBUG DEBUG>)
    target_compile_definitions(UltraSound PUBLIC $<$<CONFIG:RELEASE>:NDEBUG _NDEBUG>)
    target_link_options(UltraSound PUBLIC $<$<CONFIG:DEBUG>:DEBUG>)
    
else()

    target_compile_options(UltraSound PUBLIC $<$<CONFIG:DEBUG>:-Og -pedantic>)
    target_compile_definitions(UltraSound PUBLIC $<$<CONFIG:DEBUG>:DEBUG>)
    target_compile_options(UltraSound PUBLIC $<$<CONFIG:RELEASE>:-Ofast -march=native -mtune=native -flto>)
    target_compile_definitions(UltraSound PUBLIC $<$<CONFIG:RELEASE>:NDEBUG>)
    target_link_options(UltraSound PUBLIC $<$<CONFIG:RELEASE>: -flto>)

endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    
    target_compile_options(UltraSound PUBLIC -Wall -Wextra -fconstexpr-steps=1000000000)

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    target_compile_options(UltraSound PUBLIC -Wall -Wextra -fconstexpr-ops-limit=1000000000)

endif()



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

