/**
 * \file Interface.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "asic_call_wrapper_dll.h"

#include <cstdint>
#include <string>
#include <array>
#include <iostream>

#include "Parameters.hpp"

#ifdef _WIN32

/**
 * \brief  Type definition for the dll imported function for the asic call wrapper
 * \note   From Oldelft
 * \param  f_dllfunction:
 * \retval int: error code
 */
typedef int (_stdcall *f_dllfunction)(char *inString, char *outString);

#endif

namespace SoundCath {

/**
 * \brief  ASIC interfacer class
 * \note   A wrapper for the Oldeft API
 */
class Driver {

public:

    /**
     * \brief
     * \note
     * \retval
     */
    Driver();

    /**
     * \brief Destroy the Driver object
     * 
     */
    ~Driver();

    /**
     * \brief
     * \note
     * \param  command:
     * \retval
     */
    std::string Query(const std::string& command) const;

    /**
     * \brief 
     * 
     * \param command
     * \return DriverError 
     */
    void Send(const std::string& command) const;

    /**
     * \brief Gets the 
     * 
     * \param output: String to Output to
     */
    void Recv(std::string& output) const noexcept;

    /**
     * \brief 
     * 
     * \return string: The String the Driver Sent Back
     */
    std::string Recv() const noexcept { return GetOutString(); }

    /**
     * \brief Get the Result object
     * 
     * \return std::string 
     */
    std::string GetResult() const noexcept;

    /**
     * \brief Get the Out String object
     * 
     * \return std::string& 
     */
    std::string GetOutString() const noexcept { return std::string(outbuffer.data()); }

    /**
    * \brief Error Codes for the Driver Interface
    */
    enum Error : uint32_t {

        FAILED = 1,               ///< The Command Failed
        PARAM = 1 << 1,           ///< Bad Paramaters
        PARAMSET = 1 << 2,        ///< Bad Parameter Sets
        STATUS = 1 << 3,          ///< Driver is in a Bad State
        USB_INIT = 1 << 4,        ///< The USB Wasn't Initialized
        USB_RECEIVE = 1 << 5,     ///< The USB Received Bad Data
        USB_SEND = 1 << 6,        ///< The USB Didn't Send Correctly
        NOT_IMPLEMENTED = 1 << 7, ///< The Functionality is Not Implemented
        CRC = 1 << 8,             ///< CRC Detected a Data Error
        FPGAERROR = 1 << 9,       ///< There was an Error in the FPGA
        SWINTERNAL = 1 << 10,     ///< There was an Internal Error
        ASICERROR = 1 << 16       ///< There was an error in the ASIC

    };

    /**
     * \brief Get the Error Message object
     * 
     * \param error
     * \return constexpr const char* 
     */
    static inline constexpr const char* GetErrorMessage(const Error error) noexcept;

private:

    #ifdef _WIN32
    f_dllfunction asic_call_parse;      ///< Function pointer from the DLL
    HINSTANCE dll;                      ///< Handle for the DLL
    #endif                              

    std::array<char, UINT16_MAX> outbuffer;    ///< Data Buffer For the output data, 65kB wide

    /**
     * \brief Throws the errors cooresponding to a set uint32_t return code
     * 
     * \param error: Error Code, combination of different Errors to Parse
     * 
     * The Error Codes are stored as bits, so all of the bits that are set in the integer return code represent 
     * a different error, so we just check the bits that are set and throw the cooresponding exceptions
     */
    void ThrowErrors(const Error error) const;      

    /**
     * \brief Get the Version Command object
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* GetVersionCommand() noexcept;

    /**
     * \brief
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* InitializeCommand() noexcept;

    /**
     * \brief Set the Serial Command object
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* SetSerialCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* CloseCommand() noexcept;

};

}