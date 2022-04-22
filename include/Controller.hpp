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
template<ControllerParams::TxParams params, TransducerParams usparams>
class TXController {

public:

    /**
     * \brief Construct a new TXController object
     * 
     */
    TXController() = default;

    /**
     * \brief 
     * 
     * \return constexpr TXDelays 
     */
    constexpr Delays PreCalcDelays();

    /**
     * \brief Generate The Taylor Polynomial for the Given Parameters
     * 
     * \param[in] focalpoint: Point To Focus On
     * \param[in] beamoffset_s: 
     * \param[in] resolution_ns:  
     * \return double 
     */
    void CompressTaylor(const RectPoint& focus);

    /**
     * \brief 
     * 
     * \param x
     * \param y
     * \param z
     * \return constexpr TxCoeffs 
     */
    constexpr TxCoeffs CompressTaylor(const double x, const double y, const double z);

    /**
     * \brief 
     * 
     * \param point 
     */
    void CalculateDelays(const RectPoint& focus);

    /**
     * \brief 
     * 
     * \param x
     * \param y
     * \param z
     * \return constexpr Delays 
     */
    static constexpr Delays CalculateDelays(const double x, const double y, const double z);
    
    /**
     * \brief 
     * 
     */
    void UncompressTaylor();

    void AddDelayToQueue(const Delays& delay);

    std::queue<Delays>& GetDelayQueue() { return this->delays; }

    std::queue<TxCoeffs>& GetCoeffQueue() { return this->coeffs; };

private:

    std::queue<Delays> delays;      ///< A queue of Delays to Send to the FPGA/ASIC
    std::queue<TxCoeffs> coeffs;    ///< A Queue of Coefficients to send the FPGA/ASIC

};

struct DynRxData {

    std::array<double, 8> slope;
    std::array<double, 8> duration;
    std::array<double, 9> mastercurve;
    RxCoeffs coeffs;

};

/**
 * \brief 
 * 
 * \tparam params 
 */
template<ControllerParams::RxParams params, TransducerParams usparams>
class RXController {

public:

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
    void CompressTaylor(const RectPoint& focus, RxCoeffs& rx) const noexcept;

    /**
     * \brief 
     * \throws ControllerException
     * \param focalpoint
     */
    void CompressTaylor(const RectPoint& focus) noexcept;

    /**
     * \brief 
     * 
     * \param focus
     * \return constexpr RxCoeff 
     */
    template<double x, double y, double z>
    constexpr RxCoeffs CompressTaylor() const noexcept;

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
     * \return double 
     */
    void CompressTaylorDyn(const double x_deg, const double y_deg, const uint8_t runtime) const noexcept;

    /**
     * \brief 
     * 
     * \tparam x_deg 
     * \tparam y_deg 
     * \return constexpr DynRxData 
     */
    template<double x_deg, double y_deg, uint8_t runtime>
    constexpr DynRxData CompressTaylorDyn() const noexcept;

    /**
     * \brief 
     * 
     * \param delays
     */
    void PreCalcDelays(Delays& delays) const noexcept;

    /**
     * \brief 
     * 
     */
    constexpr Delays PreCalcDelays() const noexcept;
    
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
    void CalculateDelays(const RectPoint focus);

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

    /**
     * \brief Get the Assignment Min Max object
     * 
     * \param coeffs
     * \return constexpr std::pair<double, double> 
     */
    constexpr std::pair<double, double> GetAssignmentMinMax(const double coeffs[10]) noexcept; //

};

/**
 * \brief 
 * 
 */
template<ControllerParams params, TransducerParams usparams>
class Controller {

public:

    /**
     * \brief Construct a new Controller object
     * 
     */
    Controller() = default;

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

    TXController<params.txparams, usparams> tx;    ///< Transmssion Controller
    RXController<params.rxparams, usparams> rx;    ///< Receiving Controller
    
};

}