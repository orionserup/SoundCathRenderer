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

#include "Renderer.hpp"
#include "Ultrasound.hpp"
#include "GUI.hpp"

#include "fmt/format.h"
#include "fmt/ranges.h"

/**
 * \brief 
 * 
 * \param argc
 * \param kwargs
 * \return int 
 */
int main(const int argc, const char* const* const kwargs) {

    (void)argc;
    (void)kwargs;

    constexpr SoundCath::TransducerParams tparams{};
    constexpr SoundCath::ControllerParams conparams{};

    constexpr auto data = SoundCath::RXController<conparams.rxparams, tparams>::CompressTaylor(.005, .005, .005);

    return 0;

}