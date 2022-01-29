/**
 * \file ASIC.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-28-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "ASIC.hpp"

using namespace SoundCath;


ASIC::ASIC(Interface& face, ClkSpeed speed, Mode mode): driver(face), speed(speed), mode(mode) {}

