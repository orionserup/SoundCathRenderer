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
#include <queue>
#include <string>
#include <utility>
#include <algorithm>

#include "ASIC.hpp"
#include "FPGA.hpp"

#include "gcem.hpp"

using std::array;
using std::vector;

namespace SoundCath {

struct TxTaylor {

    TxCoeffs coeffs;
    double beamoffset_s;

};

/**
 * \brief Controller for the Transission, generates the Appropiate Delays and Coefficients
 * 
 */
template<ControllerParams::TxParams params, TransducerParams usparams>
class TXController {

public:

    /**
     * \brief 
     * 
     * \return constexpr TXDelays 
     */
    consteval Delays PreCalcDelays() noexcept;

    /**
     * \brief 
     * 
     * \param x
     * \param y
     * \param z
     * \param beamoffset_s
     * \return consteval 
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
     * \brief 
     * 
     * \tparam x 
     * \tparam y 
     * \tparam z 
     * \return Delays 
     */
    static consteval Delays CalculateDelays(const double x, const double y, const double z) noexcept {

        const double r = gcem::sqrt(gcem::pow(x, 2) + gcem::pow(y, 2) + gcem::pow(z, 2));
        Delays delays;

        for(int yg = 0; yg < usparams.ygroups; yg++) {

            double ygroup = (yg - usparams.ygroups/2 - .5) * usparams.group_pitch_nm * gcem::pow(10.0, -9.0);

            for(int xg = 0; xg < usparams.xgroups; xg++) {

                double xgroup = (xg - usparams.xgroups/2 - .5) * usparams.group_pitch_nm * gcem::pow(10.0, -9.0);
                int igroup = yg * usparams.xgroups + xg;

                for(int elx = 0; elx < usparams.xelems; elx++) {

                    double xel = xgroup + (elx - usparams.xelems/2 -.5) * usparams.pitch_nm * gcem::pow(10.0, -9);
                    for(int ely = 0; ely < usparams.yelems; ely++) {
                        int el = (elx * usparams.yelems) + ely;
                        double yel = ygroup + (ely - usparams.yelems/2 - .5) * usparams.pitch_nm * gcem::pow(10.0, -9.0);
                        delays[el + igroup * usparams.elempergroup] = -(gcem::sqrt(gcem::pow(x - xel, 2) + gcem::pow(y - yel, 2) + gcem::pow(z, 2)) - r) / (usparams.soundspeed * params.delay_res_ns * gcem::pow(10.0, -9));

                    }
                } 
            }
        }

        int8_t offset = -*std::min_element(std::begin(delays), std::end(delays));

        for(int i = 0; i < usparams.numelements; i++)
            delays[i] += offset/(params.delay_res_ns * gcem::pow(10.0, -9.0));

        return delays;

}
    
    /**
     * \brief 
     * 
     */
    static consteval Delays UncompressTaylor(const TxCoeffs coeffs);

    /**
     * \brief Get the Coeffs object
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

struct DynRxData {

    std::array<double, 8> slope;
    std::array<double, 8> duration;
    std::array<double, 9> mastercurve;
    RxCoeffs coeffs;

};

template<TransducerParams usparams>
struct RxDelays {

    Delays delays; ///< The Actual Delays Per Element in Rx Delay Res Units
    GroupDelays groupdelays; ///< Group Delay Per Group in S

};


/**
 * \brief 
 * 
 * \tparam params 
 */
template<ControllerParams::RxParams params, TransducerParams usparams>
class RXController {

private:

    static consteval std::pair<double, double> GetAssignmentMinMax(const double coeffs[7], const uint8_t scale) {

        (void)coeffs;
        (void)scale;
        return std::pair<double, double>(0.0, 0.0);

    }

public:

    /**
     * \brief 
     * 
     * \param focus
     * \return RxCoeffs: The RX Taylor Coefficients 
     */
    static consteval RxCoeffs CompressTaylor(const double x, const double y, const double z) noexcept {

        assert(x > 0 && y > 0 && z > 0);

        double r = gcem::sqrt(gcem::pow(x, 2) + gcem::pow(y, 2) + gcem::pow(z, 2));
        double x_r = x / r;
        double y_r = y / r;  

        // Find Each Term: Ripped Straight From the ASIC Spec Doc
        double seventh = (x_r / usparams.soundspeed) * (usparams.pitch_nm) * (params.c78factor/params.delay_res_ns); // 1000  factor to fix the ns and um
        double eighth = (y_r / usparams.soundspeed) * (usparams.pitch_nm) * (params.c78factor/params.delay_res_ns);
        double zeroth = (x_r / usparams.soundspeed) * (usparams.pitch_nm/params.delay_res_ns) - (floor(seventh + .5)/params.c78factor) * params.L0; 
        double first = (1 / r) * (gcem::pow(x_r, 2) - 1)/usparams.soundspeed * (1000.0 * usparams.pitch_nm) * (1000.0 * usparams.group_pitch_nm) * (1000.0 * params.L1)/(params.delay_res_ns); // scaling to prevent loss of accuracy and keeping units
        double second = (3.0 * gcem::pow(x_r, -2)) * (gcem::pow(x_r, 3) - x_r)/(2 * usparams.soundspeed) * (1000000.0 * usparams.pitch_nm * 1000.0 * usparams.group_pitch_nm * 1000.0 * usparams.group_pitch_nm * 1000000.0 * params.L2)/(params.delay_res_ns); // scaling for accuracy keeping
        double sixth  = (1 / r) * (x_r * y_r / usparams.soundspeed) * ((1000.0 * usparams.pitch_nm * 1000.0 * usparams.group_pitch_nm * 1000.0 * params.L1) / (params.delay_res_ns));
        double third = ((y_r / usparams.soundspeed) * (usparams.pitch_nm / params.delay_res_ns) - (gcem::floor(eighth + .5)/ params.c78factor)) * params.L0;
        double fourth = (1 / r) * ((gcem::pow(y_r, 2) - 1) / usparams.soundspeed) * (1000.0 * usparams.pitch_nm * 1000.0 * usparams.group_pitch_nm * 1000.0 * params.L1) / (params.delay_res_ns);
        double fifth = (3 * gcem::pow(r, -2) * gcem::pow(y_r, 3) - y_r)/(2 * usparams.soundspeed) * (10000000.0 * usparams.pitch_nm * 1000000.0 * gcem::pow(usparams.group_pitch_nm, 2) * 1000000.0 * params.L2) / params.delay_res_ns;

        double terms[] = { zeroth, first, second, third, fourth, fifth, sixth, seventh, eighth };

        int i = 1;
        for(; i <= 16; i*= 2) { // Just see the docs, its only this messy to have it evaluate at compile time
        
            if(i == 2) continue;

            if((*std::min_element(std::begin(terms), std::end(terms)) >= -128 &&
                *std::max_element(std::begin(terms), std::end(terms))  <= 127) &&
                (gcem::floor(RXController<params, usparams>::GetAssignmentMinMax(terms, i).first + .5) > 0 && 
                gcem::floor(RXController<params, usparams>::GetAssignmentMinMax(terms, i).second + .5) <= 7))
                break;
        }

        double ninth = i;

        const RxCoeffs coeffs { 

            int16_t(gcem::floor(gcem::min(gcem::max(terms[0]/ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[1]/ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[2]/ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[3]/ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[4]/ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[5]/ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[6]/ninth, -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[7], -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(terms[8], -128), 127) + .5)),
            int16_t(gcem::floor(gcem::min(gcem::max(ninth, 0), 255)) + .5)
        };

        return coeffs;

    }

    /**
     * \brief 
     * 
     * \param coeffs
     * \return consteval 
     */
    static consteval RxDelays<usparams> UncompressTaylor(const RxCoeffs coeffs) noexcept;

    /**
     * \brief 
     * 
     * \tparam x_deg 
     * \tparam y_deg 
     * \return constexpr DynRxData 
     */
    static consteval DynRxData CompressTaylorDyn(const double x_deg, const double y_deg, const uint8_t runtime) noexcept {

        assert(x_deg >= -90.0 && x_deg <= 90.0); // we need to be in the positive direction
        assert(y_deg >= -90.0 && y_deg <= 90.0); 

        const double x_rad = M_PI * x_deg / 180.0; // we need the values to be in radians for the constexpr math
        const double y_rad = M_PI * y_deg / 180.0;

        const double A = gcem::sqrt(1 - gcem::pow(gcem::sin(x_rad), 2) * gcem::pow(gcem::sin(y_rad), 2));
        const double x0 = params.start_depth_m * gcem::sin(x_rad) * gcem::cos(y_rad) / A;
        const double y0 = params.start_depth_m * gcem::sin(y_rad) * gcem::cos(x_rad) / A;
        const double z0 = params.start_depth_m * gcem::cos(x_rad) * gcem::cos(y_rad) / A;

        constexpr RxCoeffs coeffs = CompressTaylor(x0, y0, z0);
        (void)coeffs;
        DynRxData data{};
        return data;
    }

    /**
     * \brief 
     * 
     */
    static consteval Delays PreCalcDelays() noexcept;
    
    /**
     * \brief 
     * 
     */
    static consteval  void UncompressTaylorDyn(const DynRxData dynrx);

    /**
     * \brief 
     * 
     * \param focus
     * \param delays
     */
    static consteval RxDelays<usparams> CalculateDelays(const double x_deg, const double y_deg) noexcept {

        const double x_rad = x_deg * M_PI / 180.0;
        const double y_rad = y_deg * M_PI / 180.0;

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


    /**
     * \brief Get the Dyn Rx Data object
     * 
     * \return DynRxData& 
     */
    DynRxData& GetDynRxData() noexcept { return dyndata; }

    /**
     * \brief Get the Coeffs object
     * 
     * \return RxCoeffs& 
     */
    RxCoeffs& GetCoeffs() noexcept { return coeffs; }



private:
    
    DynRxData dyndata;  ///< A Queue Of Dynamic RX Data
    RxCoeffs coeffs;    ///< A Queue of Coefficients to Send to the FPGA/ASIC
    Delays delays;                ///< Delays to Send to the FPGA/ASIC
    GroupPhases phases;                ///< Phases to Send to the FPGA/ASIC

};

template<ControllerParams params, TransducerParams usparams>
struct ScanData {

    Delays txdelays[params.x_steps * params.y_steps];
    RxDelays<usparams> rxdelays[params.x_steps * params.y_steps];
    TxCoeffs txcoeffs[params.x_steps * params.y_steps];
    RxCoeffs rxcoeffs[params.x_steps * params.y_steps];

    double txoffset[params.x_steps * params.y_steps];
    GroupDelays rxgroupdelays[params.x_steps * params.y_steps];

    DynRxData dynrx[params.x_steps * params.y_steps];
    Phases rxphases[params.x_steps * params.y_steps];

};

/**
 * \brief 
 * 
 */
template<ControllerParams params, TransducerParams usparams>
class Controller {

public:

    /**
     * \brief 
     * 
     * \return consteval 
     */
    static consteval ScanData<params, usparams> PreCalcScanData() noexcept {

        ScanData<params, usparams> data;

        for(int i = 0; i < params.x_steps; i++) {
            for(int j = 0; j < params.y_steps; j++) {

                double x_deg = (params.x_max_deg - params.x_min_deg) * i / params.x_steps + params.x_min_deg;
                double y_deg = (params.y_max_deg - params.y_min_deg) * j / params.y_steps + params.y_min_deg;
                double x_rad = M_PI * x_deg / 180.0;
                double y_rad = M_PI * y_deg / 180.0;

                double A = gcem::sqrt(1 - gcem::pow(gcem::sin(x_rad), 2) * gcem::pow(gcem::sin(y_rad), 2));
                double txx = params.focus_tx * gcem::sin(x_rad) * gcem::cos(y_rad) / A;
                double txy = params.focus_tx * gcem::sin(y_rad) * gcem::sin(x_rad) / A;
                double txz = params.focus_tx * gcem::cos(x_rad) * gcem::cos(y_rad) / A;

                double rxr = params.focus_rx == 0 ? gcem::pow(10, 6): params.focus_rx;
                
                double rxx = rxr * gcem::sin(x_rad) * gcem::cos(y_rad) / A;
                double rxy = rxr * gcem::sin(y_rad) * gcem::cos(x_rad) / A;
                double rxz = rxr * gcem::cos(x_rad) * gcem::cos(y_rad) / A;
                
                if (params.usedelays) {

                    data.txcoeffs[i + j * params.x_steps] = TXController<params.txparams, usparams>::CompressTaylor(txx, txy, txz, 0).coeffs;
                    data.txoffset[i + j * params.x_steps] = TXController<params.txparams, usparams>::CompressTaylor(txx, txy, txz, 0).beamoffset_s;
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
     */
    void ProcessGroupsArray();

    /**
     * \brief 
     * 
     */
    void InitArraysGeneral();

    /**
     * \brief 
     * 
     */
    void InitArraysWet();

    /**
     * \brief Get the Time Dry object
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
    
    ScanData<params, usparams> scandata;                          ///< All of the Delays and Stuff for the whole run

};

}