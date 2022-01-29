/**
 * \file Controller.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-25-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#pragma once
#include <array>
#include "Point.hpp"

namespace SoundCath {

#define XGROUP_PITCH 4*.000180
#define YGROUP_PITCH X_PITCH

#define XMAX 7.5

#define L1_FACTOR 4
#define L2_FACTOR 64
#define L3_FACTOR 1500
#define L4_FACTOR_SQ 32

#define ELEMENT_DELAY_FP 3
#define CENTER_DELAY_FP 3
#define XGROUPS 16
#define YGROUPS 4
#define NUMGROUPS 64
#define TX_ELEMENTS 16

#define SOUND_SPEED 

using std::array;

struct TxData {

    array<int16_t, 8> txcoeffs;
    array<array<int16_t, YGROUPS>, XGROUPS> txdelays; 

};

/**
 * \brief 
 * 
 */
class TXController {

    TXController() = default;
    ~TXController();

    double CompressTaylor(const RectPoint& focalpoint, double beamoffset_s, double resolution_ns);
    void UncompressTaylor();
    void DelayUncompression(double groupdelay);

private:

    TxData data;

};

/**
 * \brief 
 * 
 */
struct RxData {

    array<int16_t, 10> rxcoeffs;

};

struct DynRxData {

    array<double, 8> slope;
    array<double, 8> duration;
    array<double, 9> mastercurve;

};


class RXController {

public:

    double CompressTaylor(RectPoint& focalpoint, double beanoffset_s, double resolution_ns);
    void UncompressTaylor();

    double CompressTaylorDyn(double x_deg, double y_deg, uint8_t runtime);
    void UncompressTaylorDyn();

    RxData& GetRxData();
    DynRxData& GetDynRxData();

private:
    
    RxData data;
    DynRxData dyndata;

}



};