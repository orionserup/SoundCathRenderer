/**
 * \file Controller.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief Contains the Implementation of the ASIC and FPGA Controller
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#include "Controller.hpp"


using SoundCath::Controller;
using SoundCath::RXController;
using SoundCath::TXController;

#ifdef USEGPU

#else

double RXController::CompressTaylor(const RectPoint& focalpoint, const double beamoffset_s, const double resolution_ns) {


    return 0.0f;

}

void RXController::UncompressTaylor() {


}

void RXController::PreCalcRxDelays() {


}

void RXController::UncompressTaylorDyn() {


}

double RXController::CompressTaylorDyn(const double x_deg, const double y_deg, const uint8_t runtime) {


    return 0.0f;

}

#endif




