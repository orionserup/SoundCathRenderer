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

#include "Point.hpp"
#include "ASIC.hpp"
#include "FPGA.hpp"

using Eigen::Matrix;
using Eigen::Vector;

using std::array;
using std::vector;

namespace SoundCath {

/**
 * \brief Controller for the Transission, generates the Appropiate Delays and Coefficients
 * 
 */
class TXController {

public:
    /**
     * \brief Construct a new TXController object
     * 
     * \param[in] max_distance_mm: Maximum Distance to Transmit to
     */
    TXController(const Params::ControllerParams& params);

    /**
     * \brief Construct a new TXController object
     * 
     */
    TXController() = default;

    /**
     * \brief Generate Delays for the Begining with empty Data
     * 
     */
    void PreCalcTxDelays();

    /**
     * \brief Generate The Taylor Polynomial for the Given Parameters
     * 
     * \param[in] focalpoint: Point To Focus On
     * \param[in] beamoffset_s: 
     * \param[in] resolution_ns:  
     * \return double 
     */
    double CompressTaylor(const RectPoint& focus);

    /**
     * \brief 
     * 
     * \param point 
     */
    void CalculateDelays(const RectPoint& focus);
    
    /**
     * \brief 
     * 
     */
    void UncompressTaylor();

    void AddDelayToQueue(const Delays& delay);

    std::queue<Delays> GetDelayQueue() const { return this->delays; }

    std::queue<TxCoeffs> GetCoeffQueue() const { return this->coeffs; };

private:

    std::queue<Delays> delays;      ///< A queue of Delays to Send to the FPGA/ASIC
    std::queue<TxCoeffs> coeffs;    ///< A Queue of Coefficients to send the FPGA/ASIC

};

struct DynRxData {

    Vector<double, 8> slope;
    Vector<double, 8> duration;
    Vector<double, 9> mastercurve;

};

class RXController {

public:

    /**
     * \brief Construct a new RXController object
     * 
     * \param maxdistance_mm 
     */
    RXController(const Params::ControllerParams& params);

    /**
     * \brief Construct a new RXController object
     * 
     */
    RXController() = default;

    /**
     * \brief 
     * \throws ControllerException
     * \param focalpoint
     * \param rx
     */
    void CompressTaylor(const RectPoint& focalpoint, RxCoeffs& rx) const;

    /**
     * \brief 
     * \throws ControllerException
     * \param focalpoint
     */
    void CompressTaylor(const RectPoint& focalpoint);

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
    double CompressTaylorDyn(double x_deg, double y_deg, const uint8_t runtime);

    /**
     * \brief 
     * 
     */
    void PreCalcRxDelays();
    
    /**
     * \brief 
     * 
     */
    void UncompressTaylorDyn();

    /**
     * \brief 
     * 
     * \param focus
     */
    void CalculateDelays(const RectPoint& focus);

    /**
     * \brief Get the Dyn Rx Data object
     * 
     * \return DynRxData& 
     */
    DynRxData& GetDynRxData();

private:
    
    DynRxData dyndata;  ///< A Queue Of Dynamic RX Data
    RxCoeffs coeffs;    ///< A Queue of Coefficients to Send to the FPGA/ASIC
    Delays delays;                ///< Delays to Send to the FPGA/ASIC
    GroupPhases phases;                ///< Phases to Send to the FPGA/ASIC

    Params::ControllerParams::RxControllerParams params;

};

/**
 * \brief 
 * 
 */
class Controller {

public:

    /**
     * \brief Construct a new Controller object
     * 
     */
    Controller();

    /**
     * \brief 
     * 
     */
    void PreCalcDelays();

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


private:

    TXController tx;    ///< Transmssion Controller
    RXController rx;    ///< Receiving Controller

    Params::ControllerParams params; ///< Parameters for Running
    
};

}