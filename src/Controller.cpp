/**
 * \file Controller.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#include "Controller.hpp"

using namespace SoundCath;

Controller::Controller(const Interface& face): asic(face), fpga(face) {}


Controller::Controller(): face(), asic(face), fpga(face) {}

