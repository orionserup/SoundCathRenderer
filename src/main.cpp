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
#include "Parameters.hpp"
#include "Ultrasound.hpp"

/**
 * \brief 
 * 
 * \param argc
 * \param kwargs
 * \return int 
 */
int main(const int argc, const char* const* const kwargs) {

    constexpr SoundCath::USParams usparams{};
    constexpr SoundCath::ControllerParams conparams{};

    SoundCath::UltraSound<usparams, conparams> us;

    SoundCath::Renderer renderer;
    SoundCath::GUI gui;
    
    return 1;

}