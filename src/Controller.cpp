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
#include "gcem.hpp"
#include <algorithm>

using SoundCath::Controller;
using SoundCath::RXController;
using SoundCath::TXController;
using SoundCath::ControllerParams;

#ifdef USEGPU

#else

template<ControllerParams::RxParams params>
void RXController<params>::CompressTaylor(const RectPoint& focus, RxCoeffs& rx) const noexcept{

    rx = CompressTaylor<focus.GetX(), focus.GetY(), focus.GetZ(), params>();

}

template<ControllerParams::RxParams params>
void RXController<params>::CompressTaylor(const RectPoint& focus) noexcept {

    CompressTaylor(focus, this->coeffs);

}

template<ControllerParams::RxParams params>
void RXController<params>::UncompressTaylor() {


}


template<ControllerParams::RxParams params>
template<double x, double y, double z>
constexpr SoundCath::RxCoeffs RXController<params>::CompressTaylor() const noexcept {

    constexpr double r = gcem::sqrt(gcem::pow(x, 2) + gcem::pow(y, 2) + gcem::pow(z, 2));
    constexpr double x_r = x / r;
    constexpr double y_r = x / r;

    constexpr double rxcoeffs[10] = {};    

    rxcoeffs[7] = (x_r/params.soundspeed) * (params.pitch_nm) * (params.c78factor/params.delay_res_ns); // 1000  factor to fix the ns and um
    rxcoeffs[8] = (y_r/params.soundspeed) * (params.pitch_nm) * (params.c78factor/params.delay_res_ns);

    rxcoeffs[0] = (x_r/params.soundspeed) * (params.pitch_nm/params.delay_res_ns) - (floor(rxcoeffs[7] + .5)/params.c78factor) * params.L0; 

    rxcoeffs[1] = (1/r) * (gcem::pow(x_r, 2) - 1)/params.soundspeed * (1000.0 * params.pitch_nm) * (1000.0 * params.group_pitch_nm) * (1000.0 * params.L1)/(params.delay_res_ns); // scaling to prevent loss of accuracy and keeping units

    rxcoeffs[2] = (3.0 * gcem::pow(x_r, -2)) * (gcem::pow(x_r, 3) - x_r)/(2 * params.soundspeed) * (1000000.0 * params.pitch_nm * 1000.0 * params.group_pitch_nm * 1000.0 * params.group_pitch_nm * 1000000.0 * params.L2)/(params.delay_res_ns); // scaling for accuracy keeping

    rxcoeffs[6] = (1 / r) * (x_r * y_r / params.soundspeed) * ((1000.0 * params.pitch_nm * 1000.0 * params.group_pitch_nm * 1000.0 * params.L1) / (params.delay_res_ns));
    rxcoeffs[3] = ((y_r / params.soundspeed) * (params.pitch_nm / params.delay_res_ns) - (gcem::floor(rxcoeffs[8] + .5)/ params.c78factor)) * params.L0;
    rxcoeffs[4] = (1 / r) * ((gcem::pow(y_r, 2) - 1) / params.soundspeed) * (1000.0 * params.pitch_nm * 1000.0 * params.group_pitch_nm * 1000.0 * params.L1) / (params.delay_res_ns);
    rxcoeffs[5] = (3 * gcem::pow(r, -2) * gcem::pow(y_r, 3) - y_r)/(2 * params.soundspeed) * (10000000.0 * params.pitch_nm * 1000000.0 * gcem::pow(params.group_pitch_nm, 2) * 1000000.0 * params.L2) / params.delay_res_ns;

    constexpr std::pair<double, double> A;

    uint8_t i;
    for(i = 1; i <= 16; i *= 2) {

        if(i == 2) continue;

        if( *std::min_element(std::begin(rxcoeffs), std::end(rxcoeffs)) * i < -128 ||
            *std::max_element(std::begin(rxcoeffs), std::end(rxcoeffs)) * i > 127)
             continue;

    }

    constexpr RxCoeffs rx;
    return rx;

}

template<ControllerParams::RxParams params>
constexpr std::pair<double, double> GetAssignmentMinMax(const double coeffs[10]) noexcept{


    return std::pair<double, double>{0.0, 0.0};

}

template<ControllerParams::RxParams params>
void RXController<params>::PreCalcDelays(Delays& delays) const noexcept{



}

template<ControllerParams::RxParams params>
void RXController<params>::UncompressTaylorDyn() {


}

template<ControllerParams::TxParams params>
void TXController<params>::CalculateDelays(const RectPoint& focus) {

    double r = std::sqrt(powf64(focus.GetX(), 2) + powf64(focus.GetY(), 2) + powf64(focus.GetZ(), 2));


}

template<ControllerParams::RxParams params>
double RXController<params>::CompressTaylorDyn(const double x_deg, const double y_deg, const uint8_t runtime) {


    return 0.0f;

}

#endif




