/**
 * \file Controller.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief Contains the Definition of the Controller Module, Controls the ASIC and FPGA to generate an Image
 * \version 0.1
 * \date 01-25-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <array>
#include <vector>
#include <string>
/// std:pair mostly
#include <utility>
/// min_element and max_element and other func stuff
#include <algorithm>

#include "ASIC.hpp"
#include "FPGA.hpp"

/// Compile Time Math Library, I have Contributed to its development and made it Windows Compatible (Generalized Compile Expression Math) same api as std::math
#include <gcem.hpp>

using std::array;
using std::vector;

namespace SoundCath {

/// Represents all of the Compressed Beam Data for Transmission
struct TxTaylor {

    TxCoeffs coeffs;        ///< The Actual Taylor Coefficients
    double beamoffset_s;    ///< The Offset of the beam for the reading

};

/**
 * \brief Controller for the Transmission, generates the Appropiate Delays and Coefficients
 * 
 * \todo Implement more functionality and Implement Taylor and Delay Storage or eliminate it
 * 
 */
template<ControllerParams::TxParams params, TransducerParams usparams>
class TXController {

public:

    /**
     * \brief Precalculates Delays
     * 
     * \note Based on the MATLAB Scripts in BMode3D
     * \todo Implement this
     * 
     * \return Delays: Precalculated delays, done at compile time
     */
    consteval Delays PreCalcDelays() noexcept;

    /**
     * \brief Generates a Taylor Polynomial for the Transmission towards a certain point In 3-D Space
     * 
     * \note See the Matlab script for clarification, I don't know how or why this works, Oldeft has dog shit documentation skills on both their API and their matlab testing code
     * \todo Test and verify this alongside the MATLAB Code
     * 
     * \param[in] x: The X Coordinate In Space
     * \param[in] y: The Y Coordinate In Space
     * \param[in] z: The Z Coordinate In Space
     * \param[in] beamoffset_s: How long to wait to receive the beam
     * 
     * \returns TxTaylor: The Taylor Polynomial \ref TxTaylor 
     */
    static consteval TxTaylor CompressTaylor(const double x, const double y, const double z, const double beamoffset_s) noexcept {

        const double r = gcem::sqrt(gcem::pow(x, 2) + gcem::pow(y, 2) + gcem::pow(z, 2));
        const double x_r = x / r;
        const double y_r = y / r;

        const double first = x_r / usparams.soundspeed * usparams.group_pitch_nm * params.L1 / params.delay_res_ns;
        const double second = 1 / r * (gcem::pow(x_r, 2) - 1) / (2 * usparams.soundspeed) * gcem::pow(usparams.group_pitch_nm, 2) * params.L2  * gcem::pow(10.0, -9) / params.delay_res_ns;
        const double third = 1 / gcem::pow(r, 2) * (gcem::pow(x_r, 3) - x_r) / (2 * usparams.soundspeed) * gcem::pow(usparams.group_pitch_nm, 3) * params.L3 * gcem::pow(10.0, -18) / params.delay_res_ns;
        const double seventh = 1 / r * x_r * y_r / usparams.soundspeed * gcem::pow(usparams.group_pitch_nm, 2) * params.L4_sq * gcem::pow(10.0, -9) / params.delay_res_ns;
        const double fourth = y_r / usparams.soundspeed * usparams.group_pitch_nm * params.L1 / params.delay_res_ns;
        const double fifth = 1 / r * (gcem::pow(y_r, 2) - 1) / (2 * usparams.soundspeed) * gcem::pow(usparams.group_pitch_nm, 2) * params.L2 * gcem::pow(10.0, -9) / params.delay_res_ns;
        const double sixth = 1 / gcem::pow(r, 2) * (gcem::pow(y_r, 3) - y_r) / (2 * usparams.soundspeed) * gcem::pow(usparams.group_pitch_nm, 3) * params.L3 * gcem::pow(10.0, -18) / params.delay_res_ns;

        const double zeromin =  gcem::floor(gcem::abs(first)) / params.L1 * params.xmax - second / params.L2 *
                                gcem::pow(params.xmax, 2) + gcem::abs(third) / params.L3 * gcem::pow(params.xmax, 3) +
                                gcem::abs(seventh) / params.L4_sq * params.xmax * params.ymax + 1.5;

        const int16_t zeroth = gcem::min(gcem::max(int16_t(gcem::floor(beamoffset_s / (params.delay_res_ns * gcem::pow(10.0, -9)) + .5)), zeromin), 255);

        const TxCoeffs coeffs {

            zeroth,
            int16_t(gcem::floor(gcem::min(gcem::max(first, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(second, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(third, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(fourth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(fifth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(sixth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(seventh, -128), 127) + .5))
    
        };

        const double beamoffset = (zeroth + 15) * params.delay_res_ns;

        return TxTaylor { coeffs, beamoffset };

    }

    /**
     * \brief Calculates the Transmission Delays to Target A Specific point
     * 
     * \note This is straight from Oldelft, Again I dont know how or why it works or even if it works
     * \todo Test and verify this side to side with the MATLAB script
     * 
     * \param[in] x: X in 3-D Space
     * \param[in] y: Y in 3-D Space
     * \param[in] z: Z in 3-D Space
     * \return Delays: The Delays that Would hit that point, evaluated   
     */
    static consteval Delays CalculateDelays(const double x, const double y, const double z) noexcept {

        const long double r = gcem::sqrt(gcem::pow(x, 2) + gcem::pow(y, 2) + gcem::pow(z, 2));
        Delays delays{};

        for(int yg = 0; yg < usparams.ygroups; yg++) {

            const long double ygroup = (yg - usparams.ygroups/2 - .5) * usparams.group_pitch_nm * gcem::pow(10.0, -9.0);

            for(int xg = 0; xg < usparams.xgroups; xg++) {

                const double xgroup = (xg - usparams.xgroups/2 - .5) * usparams.group_pitch_nm * gcem::pow(10.0, -9.0);
                const long int igroup = yg * usparams.xgroups + xg;

                for(int elx = 0; elx < usparams.xelems; elx++) {

                    const long double xel = xgroup + (elx - usparams.xelems/2 -.5) * usparams.pitch_nm * gcem::pow(10.0, -9);
                    for(int ely = 0; ely < usparams.yelems; ely++) {
                        const long int el = (elx * usparams.yelems) + ely;
                        const long double yel = ygroup + (ely - usparams.yelems/2 - .5) * usparams.pitch_nm * gcem::pow(10.0, -9.0);
                        delays[el + igroup * usparams.elempergroup] = -(gcem::sqrt(gcem::pow(x - xel, 2) + gcem::pow(y - yel, 2) + gcem::pow(z, 2)) - r) / (usparams.soundspeed * params.delay_res_ns * gcem::pow(10.0, -9));

                    }
                } 
            }
        }

        int8_t offset = -*std::min_element(std::begin(delays), std::end(delays));

        for(int i = 0; i < usparams.numelements; i++)
            delays[i] += offset/ (params.delay_res_ns * gcem::pow(10.0, -9.0));

        return delays;

}
    
    /**
     * \brief Calculates the Delays for a Given Reception Taylor Polynomial
     * 
     * \note Implement this according to Oldeft MATLAB Code
     * \todo Write this
     * 
     * \param[in] coeffs: The Coefficients to Decompress
     * \return Delays: The Caclulated Delays According to the Taylor Polynomial 
     */
    static consteval Delays UncompressTaylor(const TxCoeffs coeffs) noexcept {

        return Delays();

    }

    /**
     * \brief Get the Held Taylor 
     * 
     * \return TxCoeffs& 
     */
    TxCoeffs& GetCoeffs() noexcept { return taylor.coeffs; }

    double GetBeamOffset() noexcept { return taylor.beamoffset_s; }

    /**
     * \brief Get the Delays object
     * 
     * \return Delays& 
     */
    Delays& GetDelays() noexcept { return delays; }



private:

    Delays delays;      ///< Delays that are Calculated 
    TxTaylor taylor;    ///< Coeffs that are Calculated


};

/// Everything used for Dynamic RX, AKA Going from One Point to Another On a Ray from the Origin
struct DynRxData {

    std::array<double, 8> slope;        ///< The Slope of the Coefficients
    std::array<double, 8> duration;     ///< How Long to Hold 
    std::array<double, 9> mastercurve;  ///< The Base curve to Calculate
    RxCoeffs coeffs;                    ///< The Coefficients to Start with

};

/// Delays for the Reception, Delays for every element and delays for every group, may not want to have it be a templated struct
template<TransducerParams usparams>
struct RxDelays {

    Delays delays;           ///< The Actual Delays Per Element in Rx Delay Res Units
    GroupDelays groupdelays; ///< Group Delay Per Group in S

};

/// Recovered delays will yield a delay for every element and a phase 
template<TransducerParams usparams>
struct DecompRxDelays {

    Delays delays;  ///< Delay for every element
    Phases phases;  ///< Phase for every element, radian

};


/**
 * \brief Controls and Calculates the Delays and Taylor Coefficients Of Reception
 * 
 */
template<ControllerParams::RxParams params, TransducerParams usparams>
class RXController {
public:

    /**
     * \brief Calculates the Appropriate Taylor Polynomial to Receive at a given point
     * 
     * \note Straight from Oldelft, they are dumb and I don't know how it works
     * \todo Test and Verify and Compare against MATLAB 
     * 
     * \param[in] x: X in 3-D Space 
     * \param[in] y: Y in 3-D Space
     * \param[in] z: Z in 3-D Space
     * 
     * \return RxCoeffs: The Taylor Coefficients for reception, calculated at compile time
     */
    static consteval RxCoeffs CompressTaylor(const double x, const double y, const double z) noexcept {

        if (x == 0 && y == 0 && z == 0)
            return RxCoeffs{};

        const double r = gcem::sqrt(gcem::pow(x, 2.0) + gcem::pow(y, 2.0) + gcem::pow(z, 2.0));
        const double x_r = x / r;
        const double y_r = y / r;  

        const double pitch = usparams.pitch_nm * gcem::pow(10.0, -9);
        const double gpitch = pitch * usparams.xelems;
        const double res = params.delay_res_ns * gcem::pow(10.0, -9);
        
        const auto csound = usparams.soundspeed;

        // Find Each Term: Ripped Straight From the ASIC Spec Doc
        const double seventh = x_r / csound * pitch * params.c78factor / res; 
        const double eighth = y_r / csound * pitch * params.c78factor / res;

        const double zeroth = (x_r / csound * pitch / res - gcem::floor(seventh + .5) / params.c78factor) * params.L0; 
        const double first = 1.0 / r * (gcem::pow(x_r, 2) - 1) / csound * pitch * gpitch * params.L1 / res; // scaling to prevent loss of accuracy and keeping units
        const double second = 3.0 / gcem::pow(r, 2.0) * (gcem::pow(x_r, 3.0) - x_r) /2  / csound * pitch * gcem::pow(gpitch, 2) * params.L2 / res; // scaling for accuracy keeping
        const double sixth  = 1.0 / r * x_r * y_r / csound * pitch * gpitch * params.L1 / res;
        const double third = (y_r / csound * pitch / res - gcem::floor(eighth + .5)/ params.c78factor) * params.L0;
        const double fourth = 1.0 / r * (gcem::pow(y_r, 2) - 1) / csound * pitch * gpitch * params.L1 / res;
        const double fifth = 3.0 / gcem::pow(r, 2) * (gcem::pow(y_r, 3) - y_r)/2 /csound * pitch * gcem::pow(gpitch, 2) * params.L2 / res;

        const double terms[] = { zeroth, first, second, third, fourth, fifth, sixth, seventh, eighth };

        int i = 1;
        for(; i <= 16; i *= 2) { // Just see the docs, its only this messy to have it evaluate at compile time
        
            if (i == 2) continue;

            if ( *std::min_element(std::begin(terms), std::end(terms)) / i < -128.0 || 
                *std::max_element(std::begin(terms), std::end(terms)) / i > 127.0) 
                continue;
            
            if (gcem::floor(RXController<params, usparams>::GetAssignmentMinMax(terms, i).first + .5) >= 0.0 && 
                gcem::floor(RXController<params, usparams>::GetAssignmentMinMax(terms, i).second + .5) <= 7.0)
                break;
        }

        double ninth = i;

        const RxCoeffs coeffs { 

            int16_t(gcem::floor(gcem::min(gcem::max(terms[0] / ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[1] / ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[2] / ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[3] / ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[4] / ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[5] / ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[6] / ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[7], -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[8], -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(ninth, 0), 255) + .5))

        };

        return coeffs;

    }

    /**
     * \brief Decompresses the Taylor Coefficients into Delays and Phases
     * 
     * \note See the MATLAB script for details
     * \todo Implement this, test it, and verify it
     * 
     * \param[in] coeffs: Taylor Polynomial
     * 
     * \return RxDelays: The uncompressed coefficients
     */
    static consteval RxDelays<usparams> UncompressTaylor(const RxCoeffs coeffs) noexcept;

    /**
     * \brief Calculates the Necessary Data to Receive From a Range On a ray in a direction
     * 
     * \note Straight from Oldelft scripts, Don't know how it works
     * \todo Finish this Implementation and test/verify it against the MATLAB version
     * 
     * \param[in] x_deg: Angle From the Norm in the XZ plane
     * \param[in] y_deg: Angle From the Norm in the YZ Plane
     * \param[in] runtime: How many units of delay resolution to scan over
     * \return DynRxData: The Actual Delays/Coefficients and The Slope Over them
     */
    static consteval DynRxData CompressTaylorDyn(const double x_deg, const double y_deg, const uint8_t runtime) noexcept {

        if(!(x_deg >= -90.0 && x_deg <= 90.0) || !(y_deg >= -90.0 && y_deg <= 90.0))
            return DynRxData();

        const double x_rad = GCEM_PI * x_deg / 180.0; // we need the values to be in radians for the constexpr math
        const double y_rad = GCEM_PI * y_deg / 180.0;

        const double A = gcem::sqrt(1 - gcem::pow(gcem::sin(x_rad), 2) * gcem::pow(gcem::sin(y_rad), 2));
        const double x0 = params.start_depth_m * gcem::sin(x_rad) * gcem::cos(y_rad) / A;
        const double y0 = params.start_depth_m * gcem::sin(y_rad) * gcem::cos(x_rad) / A;
        const double z0 = params.start_depth_m * gcem::cos(x_rad) * gcem::cos(y_rad) / A;

        RxCoeffs coeffs = CompressTaylor(x0, y0, z0);
        (void)coeffs;
        DynRxData data{};
        return data;
    }

    /**
     * \brief Precalculates the Delays for Receiving 
     * 
     * \note See the MATLAB Script for implementation details
     * \todo Implement everything
     * 
     */
    static consteval Delays PreCalcDelays() noexcept {

        return Delays();

    }
    
    /**
     * \brief Takes Dynamic Compression Data and Gets Delays From it
     * 
     * \note Implemented based on the Oldeft MATLAB Scripts
     * \todo Actually Implement this
     * 
     */
    static consteval void UncompressTaylorDyn(const DynRxData dynrx) {

        

    }

    /**
     * \brief Generates compressed Taylor Polynomials for reading from a given direction
     * 
     * \note Straight from Oldelft MATLAB scripts, don't know or follow the logic
     * \todo Test and verify the Algorithm as well as finish it according to the script
     * 
     * \param[in] x_deg: The Angle from the XZ plane
     * \param[in] y_deg: The angle from the XY plane
     * \return RxDelays: The Appropiate Taylor Polynomial data for receiving from the x and y degrees ray direction 
     */
    static consteval RxDelays<usparams> CalculateDelays(const double x_deg, const double y_deg) noexcept {

        const double x_rad = x_deg * GCEM_PI / 180.0;
        const double y_rad = y_deg * GCEM_PI / 180.0;

        RxDelays<usparams> delays{};

        for(int x = 0; x < usparams.xelems; x++) {

            const double elx = (x - usparams.xelems/2 - .5) * usparams.pitch_nm * gcem::pow(10.0, -9);
            for(int y = 0; y < usparams.yelems; y++) {
                const int el = y + usparams.yelems * x;
                const double ely = (y - usparams.yelems / 2 - .5) * usparams.pitch_nm * gcem::pow(10.0, -9);

                for (int g = 0; g < usparams.numgroups; g++)
                    delays.delays[el + g * usparams.elempergroup] = ((elx * gcem::sin(x_rad) + ely * gcem::sin(y_rad)) / (usparams.soundspeed * params.delay_res_ns * gcem::pow(10.0, -9))) + 14;

            }
        }

        const double a = gcem::sqrt(1 - gcem::pow(gcem::sin(x_rad), 2) * gcem::pow(gcem::sin(y_rad), 2));
        const double fx = gcem::sin(x_rad) * gcem::cos(y_rad) / a;
        const double fy = gcem::sin(y_rad) * gcem::cos(x_rad) / a;
        const double fz = gcem::cos(x_rad) * gcem::cos(y_rad) / a;

        return delays;

    }
    
private:
    
    DynRxData dyndata;      ///< Dynamic RX Data if its being used
    RxCoeffs coeffs;        ///< Coefficients to Send to the FPGA/ASIC
    Delays delays;          ///< Delays to Send to the FPGA/ASIC
    GroupDelays groupphases;///< Phases to Send to the FPGA/ASIC


    /**
     * \brief Get the Assignment Min Max, it is a helper for some of the functions, see the MATLAB Code
     * 
     * \note Don't really know how or why it is supposed to work 
     * \todo Implement and Test it
     * 
     * \param[in] coeffs: Coefficients to sort and minmax
     * \param[in] scale: the value to divide the coeffs by
     * 
     * \return std::pair, the min and max
     */
    static consteval std::pair<double, double> GetAssignmentMinMax(const double coeffs[7], const uint8_t scale) {

        (void)coeffs;
        (void)scale;
        return std::pair<double, double>(0.0, 0.0);

    }


};

/**
 * \brief All of the Data that is Needed to Sweep over a given 3-D Volume defined By the template Parameters
 * 
 * \tparam params: Controller Related Parameters including x and y steps 
 * \tparam usparams: Physical Parameters for the Ultrasound 
 */
template<ControllerParams params, TransducerParams usparams>
struct ScanData {

    std::array<Delays, params.x_steps * params.y_steps> txdelays;               ///< Tx Delays Over that Scan Area
    std::array<RxDelays<usparams>, params.x_steps * params.y_steps> rxdelays;   ///< Rx Delays Over the Scan Area
    std::array<TxCoeffs, params.x_steps * params.y_steps> txcoeffs;             ///< Tx Taylor Coefficients Over the Scan Area
    std::array<RxCoeffs, params.x_steps * params.y_steps> rxcoeffs;             ///< Rx Taylor Coefficients Over the Scan Area

    std::array<double, params.x_steps * params.y_steps> txoffsets;              ///< Transmissions Offsets Over the Scan Area
    std::array<GroupDelays, params.x_steps * params.y_steps> rxgroupdelays;     ///< Group Delays over the scan area for reception

    std::array<DynRxData, params.x_steps * params.y_steps> dynrx;               ///< Dynamic RX Curves for the whole scan area
    std::array<Phases, params.x_steps * params.y_steps> phases;                 ///< Phases over the Scan Volume

};

/**
 * \brief Controller that Manages both RX and TX
 * 
 * \tparam params: Controller Parameters, how and where and when we want to scan
 * \tparam usparams: The Physical Parameters
 */
template<ControllerParams params, TransducerParams usparams>
class Controller {

public:

    /**
     * \brief Calculates the Scan Data for the whole scan range, all of the Coefficents, Delays, or Dynamic Data depending on the Controller Paramaters
     * 
     * \note Just Calculates Coeffs or Delays or Dynamic Data based on Controller Parameters
     * \todo Test and Verify this
     * 
     * \return ScanData: Enough Data to go over the whole scan volume
     */
    static consteval ScanData<params, usparams> PreCalcScanData() noexcept {

        ScanData<params, usparams> data;

        for(int i = 0; i < params.x_steps; i++) {
            for(int j = 0; j < params.y_steps; j++) {

                double x_deg = (params.x_max_deg - params.x_min_deg) * i / params.x_steps + params.x_min_deg;
                double y_deg = (params.y_max_deg - params.y_min_deg) * j / params.y_steps + params.y_min_deg;
                double x_rad = GCEM_PI * x_deg / 180.0;
                double y_rad = GCEM_PI * y_deg / 180.0;

                double A = gcem::sqrt(1 - gcem::pow(gcem::sin(x_rad), 2) * gcem::pow(gcem::sin(y_rad), 2));
                double txx = params.focus_tx * gcem::sin(x_rad) * gcem::cos(y_rad) / A;
                double txy = params.focus_tx * gcem::sin(y_rad) * gcem::sin(x_rad) / A;
                double txz = params.focus_tx * gcem::cos(x_rad) * gcem::cos(y_rad) / A;

                double rxr = params.focus_rx == 0 ? gcem::pow(10, 6): params.focus_rx;
                
                double rxx = rxr * gcem::sin(x_rad) * gcem::cos(y_rad) / A;
                double rxy = rxr * gcem::sin(y_rad) * gcem::cos(x_rad) / A;
                double rxz = rxr * gcem::cos(x_rad) * gcem::cos(y_rad) / A;
                
                if constexpr (!params.usedelays) {

                    data.txcoeffs[i + j * params.x_steps] = TXController<params.txparams, usparams>::CompressTaylor(txx, txy, txz, 0).coeffs;
                    data.rxcoeffs[i + j * params.x_steps] = RXController<params.rxparams, usparams>::CompressTaylor(txx, txy, txz);
                    data.txoffsets[i + j * params.x_steps] = TXController<params.txparams, usparams>::CompressTaylor(txx, txy, txz, 0).beamoffset_s;
                    data.rxgroupdelays[i + j * params.x_steps] = RXController<params.rxparams, usparams>::CalculateDelays(x_deg, y_deg).groupdelays;

                }
                else {

                    data.txdelays[i + j * params.x_steps] = TXController<params.txparams, usparams>::CalculateDelays(txx, txy, txz);
                    data.rxdelays[i + j * params.x_steps] = RXController<params.rxparams, usparams>::CalculateDelays(x_deg, y_deg);
                    
                }
            }
        }

        return data;

    }

    /**
     * \brief 
     * 
     * \todo Implement this
     * 
     */
    void ProcessGroupsArray();

    /**
     * \brief 
     * 
     * \todo Implement this
     * 
     */
    void InitArraysGeneral();

    /**
     * 
     * \brief 
     * 
     * \todo Implement this
     * 
     */
    void InitArraysWet();

    /**
     * \brief Get the Time Dry object
     * 
     * \todo Implement this
     * 
     * \return double 
     */
    double GetTimeDry();
    
    /**
     * \brief Get the Scan Data object
     * 
     * \return ScanData& 
     */
    auto& GetScanData() noexcept { return scandata; }


private:

    TXController<params.txparams, usparams> tx;    ///< Transmssion Controller
    RXController<params.rxparams, usparams> rx;    ///< Receiving Controller
    
    ScanData<params, usparams> scandata;           ///< All of the Delays and Stuff for the whole run

};

}