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

    SoundCath::ParameterParser parser(argc, kwargs);
    SoundCath::Params param = parser.GetParams();

    SoundCath::UltraSound us;

    SoundCath::Renderer renderer;
    render.AddUltraSound(us);

    renderer.Render();


    return 1;

}