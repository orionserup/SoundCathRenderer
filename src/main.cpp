/**
 * \file main.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <chrono>

#include "Renderer.hpp"
#include "Ultrasound.hpp"
#include "GUI.hpp"

#include "fmt/format.h"
#include "fmt/ranges.h"
#include "fmt/chrono.h"

using SoundCath::RXController;

/// \todo Write the main to use the ultrsound and render it to a gui
/// \todo Make sure that the compiler can handle all of the constexpr, needs more than 12GB Ram Currently
/// \todo Implement a command line argument parser and setup options, there are a few libraries that will help

/**
 * \brief The Program entry point
 * 
 * \param argc: The Number of Arguments
 * \param kwargs: The Arguments
 * \return int: The exit status code
 */
int main(const int argc, const char* const* const kwargs) {

    (void)argc;
    (void)kwargs;

    constexpr SoundCath::TransducerParams tparams{};
    constexpr SoundCath::ControllerParams conparams{};

    auto start = std::chrono::high_resolution_clock::now();

    static constinit std::array<SoundCath::RxCoeffs, 10> data{
        SoundCath::RXController<conparams.rxparams, tparams>::CompressTaylor(.005, .005, .005),
        SoundCath::RXController<conparams.rxparams, tparams>::CompressTaylor(.01, .005, .005),
        SoundCath::RXController<conparams.rxparams, tparams>::CompressTaylor(.01, .01, .005),
        SoundCath::RXController<conparams.rxparams, tparams>::CompressTaylor(.01, .01, .01),
        SoundCath::RXController<conparams.rxparams, tparams>::CompressTaylor(.02, .02, .02),
        SoundCath::RXController<conparams.rxparams, tparams>::CompressTaylor(.044, .06, .04),
        SoundCath::RXController<conparams.rxparams, tparams>::CompressTaylor(.066, .05, .01),
        SoundCath::RXController<conparams.rxparams, tparams>::CompressTaylor(.07, .099, .1),
        SoundCath::RXController<conparams.rxparams, tparams>::CompressTaylor(.003, .005, .5),
        SoundCath::RXController<conparams.rxparams, tparams>::CompressTaylor(.001, .002, .003)
    };
    
    static constinit std::array<SoundCath::RxDelays<tparams>, 10> delaydata { 
        RXController<conparams.rxparams, tparams>::CalculateDelays(10, 10),
        RXController<conparams.rxparams, tparams>::CalculateDelays(10, 20),
        RXController<conparams.rxparams, tparams>::CalculateDelays(10, 50),
        RXController<conparams.rxparams, tparams>::CalculateDelays(6, 10),
        RXController<conparams.rxparams, tparams>::CalculateDelays(12, 40),
        RXController<conparams.rxparams, tparams>::CalculateDelays(67, 65),
        RXController<conparams.rxparams, tparams>::CalculateDelays(10, 21),
        RXController<conparams.rxparams, tparams>::CalculateDelays(1, 10),
        RXController<conparams.rxparams, tparams>::CalculateDelays(40, 10),
        RXController<conparams.rxparams, tparams>::CalculateDelays(6, 30)
    };    

    static constinit auto scandata = SoundCath::Controller<conparams, tparams>::PreCalcScanData();
    
    auto stop = std::chrono::high_resolution_clock::now();

    for (auto& tp: data)
        fmt::print("{}\n", tp);

    for (auto& delays: delaydata)
        fmt::print("{}\n", delays.delays);

    for (auto& scanrx: scandata.txdelays)
        fmt::print("{}\n", scanrx);

    fmt::print("Finished In {}", stop - start);

    return 0;

}