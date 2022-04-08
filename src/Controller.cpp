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
#include "Constants.hpp"

#include <cmath>

using SoundCath::Controller;
using SoundCath::RXController;
using SoundCath::TXController;

#ifdef USEGPU

#else

void RXController::CompressTaylor(const RectPoint& focalpoint, RxCoeffs& rx) const {

    double r = sqrt(pow(focalpoint.GetX(), 2) + pow(focalpoint.GetY(), 2) + pow(focalpoint.GetZ(), 2));
    double x_r = focalpoint.GetX() / r;
    double y_r = focalpoint.GetY() / r;

    std::array<double, 10> rxcoeffs;    

    rxcoeffs[7] = (x_r/params.soundspeed) * (params.pitch_nm) * (params.c78factor/params.delay_res_ns); // 1000  factor to fix the ns and um
    rxcoeffs[8] = (y_r/params.soundspeed) * (params.pitch_nm) * (params.c78factor/params.delay_res_ns);

    //  Taylor around X=Y=0
    //  dX
    //  F = -1 * ( (X-Fx)^2 + (Y-Fy)^2 + Fz^2 )^(-0.5) * 0.5 * 2 * (X-Fx);
    //  A = F(a) (X-a) = 1/R * R*sin(ax);
    rxcoeffs[0] = (x_r/params.soundspeed) * (params.pitch_nm/params.delay_res_ns) - (floor(rxcoeffs[7] + .5)/params.c78factor) * params.L0; 
    
    // Resolution of dX is 4 times higher (see c9/4 in formula)
    // First derivative
    // Fx = -1 * ( (X-Fx)^2 + (Y-Fy)^2 + Fz^2 )^(-1.5) * -0.5 * (X-Fx) * 2 * (X-Fx) + ...
    // 
    // -1 * ( (X-Fx)^2 + (Y-Fy)^2 + Fz^2 )^(-0.5);
    // Ax = Fx(a)/factorial(1) * (X-a)^2 = (1/R^3 * R^2*sin(ax)^2 - 1/R) * X^2 = ...
    // 
    // 1/R * (sin(ax)^2 -1) *X^2;
    rxcoeffs[1] = (1/r) * (x_r * x_r - 1)/params.soundspeed * (1000.0 * params.pitch_nm) * (1000.0 * params.group_pitch_nm) * (1000.0 * params.L1)/(params.delay_res_ns); // scaling to prevent loss of accuracy and keeping units

    //  Fxx = -3 * ( (X-Fx)^2 + (Y-Fy)^2 + Fz^2 )^(-2.5) * (X-Fx)^3 + 3 * ( (X-Fx)^2 + (Y-
    // Fy)^2 + Fz^2 )^(-1.5) * (X-Fx);
    // Axx = f_dXdXdX(a)/factorial(2) * (X-a)^3 = (3/R^2* (sin(ax))^3 - 3/R^2 * sin(ax))/2
    // * X^3 = ...
    // 
    //  3/R^2 * ( sin(ax)^3 - sin(ax)) /2 *
    // X^3;
    rxcoeffs[2] = (3.0/powf64(r, 2)) * ((powf64(x_r, 3) - x_r)/(2 * params.soundspeed)) * ((1000000.0 * params.pitch_nm * 1000.0 * params.group_pitch_nm * 1000.0 * params.group_pitch_nm * 1000000.0 * params.L2)/(params.L2)); // scaling for accuracy keeping

    rxcoeffs[6] = (1 / r) * (x_r * y_r/params.soundspeed) * ((1000.0 * params.pitch_nm * 1000.0 * params.group_pitch_nm * 1000.0 * params.L1) / (params.delay_res_ns));
    rxcoeffs[3] = 

}

void RXController::UncompressTaylor() {


}

void RXController::PreCalcRxDelays() {


}

void RXController::UncompressTaylorDyn() {


}

void TXController::CalculateDelays(const RectPoint& focus) {

    double r = sqrtf64(powf64(focus.GetX(), 2) + powf64(focus.GetY(), 2) + powf64(focus.GetZ(), 2));

    for(auto y = this->delays) {

    }


}

double RXController::CompressTaylorDyn(const double x_deg, const double y_deg, const uint8_t runtime) {


    return 0.0f;

}

#endif




