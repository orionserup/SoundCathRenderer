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

#include <cstdint>
#include <string>
#include <array>
#include <iostream>

#include "Parameters.hpp"

#ifdef _WIN32
#include <windows.h>

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
 * \brief Errors caused from the Driver/Interface
 * 
 */
struct DriverError {

    /**
    * \brief Error Codes for the Driver Interface
    */
    enum Code : uint32_t {
        OK = 0,                    ///< Command Went Smoothly
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

    Code error;     ///< The Actual Error Code

    /**
     * \brief Get the Error Message From the Error Code
     * 
     * \param[in] error
     * \return const char*: The Error Message as a String
     */
    static const char* GetErrorMessage(const Code error) noexcept;

    /**
     * \brief Throws the errors cooresponding to a set uint32_t return code
     * \throws DriverException: Because thats the Point
     * \param[in] error: Error Code, combination of different Errors to Parse
     * 
     * The Error Codes are stored as bits, so all of the bits that are set in the integer return code represent 
     * a different error, so we just check the bits that are set and throw the cooresponding exceptions
     */
    static void ThrowErrors(const Code error); 

};

/**
 * \brief  ASIC interfacer class
 * \note   A wrapper for the Oldeft API
 */
class Driver {

public:

    /**
     * \brief Construct a new Driver object
     * 
     */
    Driver();

    /**
     * \brief Destroy the Driver object
     * 
     */
    ~Driver();

    /**
     * \brief Sends A Command String and Returns what The Interface Sent Back
     * \throws DriverException: If there are Issues on the Backend
     * \param[in] command: Command To Send the Device
     * \return std::string: What The Device Sent Back
     */
    std::string Query(const std::string& command) const;

    /**
     * \brief Sends A Command String To The Interface Box
     * \throws DriverException: If there are any issues On the Backend
     * \param[in] command: Command String To Send
     */
    void Send(const std::string& command) const;

    /**
     * \brief Receives the Last Output to an External String
     * 
     * \param[out] output: String to Receieve to 
     */
    void Recv(std::string& output) const noexcept;

    /**
     * \brief Gets the Last Received Response
     * 
     * \return string: The String the Driver Sent Back
     */
    std::string Recv() const noexcept { return GetOutString(); }

    /**
     * \brief Gets the Reponse String From the Last Output (ie all of the important Data)
     * 
     * \return std::string: A substring of the output string without echo of the send string
     */
    std::string GetResult() const noexcept;

    /**
     * \brief Get the Output String of the Last Command Sent
     * 
     * \return std::string: The Reponse From The Last Command
     */
    std::string GetOutString() const noexcept { return std::string(outbuffer.data()); }

private:

    #ifdef _WIN32
    f_dllfunction asic_call_parse;      ///< Function pointer from the DLL
    HINSTANCE dll;                      ///< Handle for the DLL
    #endif                              

    std::array<char, UINT16_MAX> outbuffer;    ///< Data Buffer For the output data, 65kB wide     

};

}