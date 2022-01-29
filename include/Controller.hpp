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
#include <vector>

#include "Point.hpp"
#include "Constants.hpp"

namespace SoundCath {

using std::array;
using std::vector;

/// Group Delay Types
typedef array<int16_t, GROUPELEMENTS> GroupDelays;
typedef array<array<GroupDelays, YGROUPS>, XGROUPS + 1> Delays;
typedef Delays Phases;

/// Taylor Coefficient Types
typedef array<int16_t, 8> TxCoeffs;
typedef array<int16_t, 10> RxCoeffs;


/**
 * \brief 
 * 
 */
class TXController {

    /**
     * \brief Construct a new TXController object
     * 
     */
    TXController() = default;

    /**
     * \brief Destroy the TXController object
     * 
     */
    ~TXController();

    /**
     * \brief 
     * 
     * \param focalpoint 
     * \param beamoffset_s 
     * \param resolution_ns 
     * \return double 
     */
    double CompressTaylor(const RectPoint& focalpoint, double beamoffset_s, double resolution_ns);
    
    /**
     * \brief 
     * 
     */
    void UncompressTaylor();
    
    /**
     * \brief 
     * 
     * \param groupdelay 
     */
    void DelayUncompression(double groupdelay);

private:

    std::vector<Delays> txdelays;
    TxCoeffs coeffs;

};

struct DynRxData {

    array<double, 8> slope;
    array<double, 8> duration;
    array<double, 9> mastercurve;

};

class RXController {

public:

    /**
     * \brief 
     * 
     * \param focalpoint 
     * \param beanoffset_s 
     * \param resolution_ns 
     * \param csound 
     * \return double 
     */
    double CompressTaylor(RectPoint& focalpoint, double beanoffset_s, double resolution_ns, double csound = 1520.0f);
    /**
     * \brief 
     * 
     */
    void UncompressTaylor();

    /**
     * \brief 
     * 
     * \param x_deg 
     * \param y_deg 
     * \param runtime 
     * \return double 
     */
    double CompressTaylorDyn(double x_deg, double y_deg, uint8_t runtime);
    
    /**
     * \brief 
     * 
     */
    void UncompressTaylorDyn();

    /**
     * \brief Get the Dyn Rx Data object
     * 
     * \return DynRxData& 
     */
    DynRxData& GetDynRxData();

private:
    
    RxCoeffs coeffs;
    DynRxData dyndata;
    vector<Delays> delays;
    vector<Phases> phases;

};

class Controller {

public:

    /**
     * \brief Construct a new Controller object
     * 
     */
    Controller();

    /**
     * \brief Destroy the Controller object
     * 
     */
    ~Controller();



private:

    TXController tx;
    RXController rx;
    
    

};

};