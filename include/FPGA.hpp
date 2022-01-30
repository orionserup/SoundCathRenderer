/**
 * \file FPGA.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "Interface.hpp"
#include <string>

using std::string;

namespace SoundCath {

/**
 * \brief 
 * 
 */
class FPGA {

public:

    /**
     * \brief Construct a new FPGA object
     * 
     * \param face 
     */
    FPGA(const Interface& face);

    /**
     * \brief Get the Version object
     * 
     * \return string 
     */
    string GetVersion();

    /**
     * \brief Get the Description object
     * 
     * \return string 
     */
    string GetDescription();

    /**
    * \brief 
    * 
    */
    enum Error : uint32_t {

        QSPI = 1 << 0,          ///< Quad SPI Error
        CLOCK = 1 << 3,         ///< Clock Not Synced
        FPGA_POW = 1 << 4,      ///< Power is Not Good
        HV_POW = 1 << 5,        ///< High Voltage Power Source is Not Good
        LV_POW = 1 << 6,        ///< Low Voltage Power Source is Not Good
        POW_FAIL = 1 << 7,      ///< Power Failure Shutdown
        CMD = 1 << 8,           ///< Command Error
        TRIGGER = 1 << 9,       ///< Trigger Error
        CLKBUSY = 1 << 10,      ///< Clock is Busy
        OVERFULL  = 1 << 16,    ///< The Memory is too full
        FRAME_ERROR = 1 << 17   ///< Frame Memory Error

    };

    /**
     * \brief Get the Error Message object
     * 
     * \param error
     * \return constexpr const char* 
     */
    static constexpr const char* GetErrorMessage(const FPGA::Error error) noexcept;

private:

    Interface& face;
    string version;
    string desc;

};

}
