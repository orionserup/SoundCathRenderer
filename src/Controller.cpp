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
using SoundCath::TransducerParams;

#ifdef USEGPU

#else

template<ControllerParams::RxParams params, TransducerParams usparams>
void RXController<params, usparams>::CompressTaylor(const RectPoint& focus, RxCoeffs& rx) const noexcept{

    rx = CompressTaylor<focus.GetX(), focus.GetY(), focus.GetZ(), params>();

}

template<ControllerParams::RxParams params, TransducerParams usparams>
void RXController<params, usparams>::CompressTaylor(const RectPoint& focus) noexcept {

    CompressTaylor(focus, this->coeffs);

}

template<ControllerParams::RxParams params, TransducerParams usparams>
void RXController<params, usparams>::UncompressTaylor() {


}


template<ControllerParams::RxParams params, TransducerParams usparams>
template<double x, double y, double z>
constexpr SoundCath::RxCoeffs RXController<params, usparams>::CompressTaylor() const noexcept {

    constexpr double r = gcem::sqrt(gcem::pow(x, 2) + gcem::pow(y, 2) + gcem::pow(z, 2));
    constexpr double x_r = x / r;
    constexpr double y_r = x / r;  

    // Find Each Term: Ripped Straight From the ASIC Spec Doc
    constexpr double seventh = (x_r/usparams.soundspeed) * (usparams.pitch_nm) * (params.c78factor/params.delay_res_ns); // 1000  factor to fix the ns and um
    constexpr double eighth = (y_r/usparams.soundspeed) * (usparams.pitch_nm) * (params.c78factor/params.delay_res_ns);
    constexpr double zeroth = (x_r/usparams.soundspeed) * (usparams.pitch_nm/params.delay_res_ns) - (floor(seventh + .5)/params.c78factor) * params.L0; 
    constexpr double first = (1/r) * (gcem::pow(x_r, 2) - 1)/usparams.soundspeed * (1000.0 * usparams.pitch_nm) * (1000.0 * usparams.group_pitch_nm) * (1000.0 * params.L1)/(params.delay_res_ns); // scaling to prevent loss of accuracy and keeping units
    constexpr double second = (3.0 * gcem::pow(x_r, -2)) * (gcem::pow(x_r, 3) - x_r)/(2 * usparams.soundspeed) * (1000000.0 * usparams.pitch_nm * 1000.0 * usparams.group_pitch_nm * 1000.0 * usparams.group_pitch_nm * 1000000.0 * params.L2)/(params.delay_res_ns); // scaling for accuracy keeping
    constexpr double sixth  = (1 / r) * (x_r * y_r / usparams.soundspeed) * ((1000.0 * usparams.pitch_nm * 1000.0 * usparams.group_pitch_nm * 1000.0 * params.L1) / (params.delay_res_ns));
    constexpr double third = ((y_r / usparams.soundspeed) * (usparams.pitch_nm / params.delay_res_ns) - (gcem::floor(eighth + .5)/ params.c78factor)) * params.L0;
    constexpr double fourth = (1 / r) * ((gcem::pow(y_r, 2) - 1) / usparams.soundspeed) * (1000.0 * usparams.pitch_nm * 1000.0 * usparams.group_pitch_nm * 1000.0 * params.L1) / (params.delay_res_ns);
    constexpr double fifth = (3 * gcem::pow(r, -2) * gcem::pow(y_r, 3) - y_r)/(2 * usparams.soundspeed) * (10000000.0 * usparams.pitch_nm * 1000000.0 * gcem::pow(usparams.group_pitch_nm, 2) * 1000000.0 * params.L2) / params.delay_res_ns;

    constexpr double terms[] = { zeroth, first, second, third, fourth, fifth, sixth, seventh, eighth };

    uint8_t i;
    for(i = 1; i <= 16; i *= 2) {

        if(i == 2) continue;

        if( *std::min_element(std::begin(terms), std::end(terms)) * i < -128 ||
            *std::max_element(std::begin(terms), std::end(terms)) * i > 127)
             continue;

    }

    constexpr RxCoeffs rx{};
    return rx;

}

template<ControllerParams::RxParams params, TransducerParams usparams>
constexpr std::pair<double, double> RXController<params, usparams>::GetAssignmentMinMax(const double coeffs[10]) noexcept{

    (void)coeffs;
    return std::pair<double, double>{0.0, 0.0};

}

using SoundCath::DynRxData;

template<ControllerParams::RxParams params, TransducerParams usparams>
template<double x_deg, double y_deg, uint8_t runtime>
constexpr DynRxData RXController<params, usparams>::CompressTaylorDyn() const noexcept {

    static_assert(x_deg >= -90.0 && x_deg <= 90.0); // we need to be in the positive direction
    static_assert(y_deg >= -90.0 && y_deg <= 90.0); 

    constexpr double x_rad = M_PI * x_deg / 180.0; // we need the values to be in radians for the constexpr math
    constexpr double y_rad = M_PI * y_deg / 180.0;

    constexpr double A = gcem::sqrt(1 - gcem::pow(gcem::sin(x_rad), 2) * gcem::pow(gcem::sin(y_rad), 2));
    constexpr double x0 = params.start_depth_m * gcem::sin(x_rad) * gcem::cos(y_rad) / A;
    constexpr double y0 = params.start_depth_m * gcem::sin(y_rad) * gcem::cos(x_rad) / A;
    constexpr double z0 = params.start_depth_m * gcem::cos(x_rad) * gcem::cos(y_rad) / A;

    constexpr RxCoeffs coeffs = CompressTaylor<x0, y0, z0>();
    (void)coeffs;


    constexpr DynRxData data{};
    return data;

}

template<ControllerParams::RxParams params, TransducerParams usparams>
void RXController<params, usparams>::CompressTaylorDyn(const double x_deg, const double y_deg, const uint8_t runtime) const noexcept {

    this->dyndata = CompressTaylorDyn<x_deg, y_deg, runtime>();

}



template<ControllerParams::RxParams params, TransducerParams usparams>
void RXController<params, usparams>::PreCalcDelays(Delays& delays) const noexcept{

    delays = PreCalcDelays();

}

template<ControllerParams::RxParams params, TransducerParams usparams>
constexpr SoundCath::Delays RXController<params, usparams>::PreCalcDelays() const noexcept {

    return SoundCath::Delays();

}

template<ControllerParams::RxParams params, TransducerParams usparams>
void RXController<params, usparams>::UncompressTaylorDyn() {


}

template<ControllerParams::TxParams params, TransducerParams usparams>
void TXController<params, usparams>::CalculateDelays(const RectPoint& focus) {

    double r = std::sqrt(powf64(focus.GetX(), 2) + powf64(focus.GetY(), 2) + powf64(focus.GetZ(), 2));
    (void)r;

}



#endif




