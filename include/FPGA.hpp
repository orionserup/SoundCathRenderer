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

#include "Driver.hpp"
#include <string>

namespace SoundCath {

struct FPGAError {

    /**
    * \brief 
    * 
    */
    enum Code: uint32_t {

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

    Code error;         ///< The Actual Error Code

    /**
     * \brief Get the Error Message object
     * 
     * \param error
     * \return constexpr const char* 
     */
    static const char* GetErrorMessage(const Code error) noexcept;

    /**
     * \brief 
     * 
     * \param code
     */
    static void ThrowErrors(const Code code);

};

/**
 * \brief 
 * 
 */
template<FPGAParams params>
class FPGA {

public:

    /**
     * \brief Construct a new FPGA object
     * 
     * \param face 
     */
    FPGA(Driver& face);

    /**
     * \brief Get the Version object
     * 
     * \return string 
     */
    std::string GetVersion() const { return this->version; }

    /**
     * \brief Get the Description object
     * 
     * \return string 
     */
    std::string GetDescription() const { return this->desc; }

    /**
     * \brief Get the Error object
     * 
     * \return Error 
     */
    FPGAError::Code GetError() const;


private:

    Driver& face;            ///< The Physical Interface that is Connected to the FPGA
    std::string version;     ///< The HDL Version
    std::string desc;        ///< Physical Description

};

}
