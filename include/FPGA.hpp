/**
 * \file FPGA.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief Contains all of class definitions and declarations for the FPGA Class
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

/**
 * \brief Errors from the FPGA
 * 
 */
struct FPGAError {

    /**
    * \brief The Bitmask Flags 
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
     * \param error: The error code
     * \return constexpr const char*: What the error message is 
     */
    static const char* GetErrorMessage(const Code error) noexcept;

    /**
     * \brief Throws various errors according to the error code
     * 
     * \param[in] code: The error code to interpret and throw corresponding errors
     */
    static void ThrowErrors(const Code code);

};

/**
 * \brief FPGA Class to Interface with the FPGA in the Box
 * 
 * \tparam params: Parameters for the FPGA
 */
template<FPGAParams params>
class FPGA {

public:

    /**
     * \brief Construct a new FPGA object
     * 
     * \param[in] face: The Oldeft Interface to Interact from
     */
    FPGA(Driver& face);

    /**
     * \brief Get the Version object
     * 
     * \return string: The version 
     */
    std::string GetVersion() const { return this->version; }

    /**
     * \brief Get the Description object
     * 
     * \return string: The Device description
     */
    std::string GetDescription() const { return this->desc; }

    /**
     * \brief Get the Error object
     * 
     * \return Code: the Thrown error
     */
    FPGAError::Code GetError() const;


private:

    Driver& face;            ///< The Physical Interface that is Connected to the FPGA
    std::string version;     ///< The HDL Version
    std::string desc;        ///< Physical Description

};

}
