/**
 * \file Ultrasound.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "Controller.hpp"

namespace SoundCath {
    
    template<USParams params, ControllerParams conparams>
    class UltraSound {


    private:

        Controller<conparams> controller;

    };

}