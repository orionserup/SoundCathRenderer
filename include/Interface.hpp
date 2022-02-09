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
#include <memory>

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
 * \brief 
 * 
 */
enum Parameter : uint16_t {



};

/**
 * \brief  ASIC interfacer class
 * \note   A wrapper for the Oldeft API
 */
class Interface {

public:

    /**
     * \brief 
     * 
     */
    class Config {

    public:

        /**
         * \brief Construct a new Config object
         * 
         */
        Config();



    private:



    };

    /**
     * \brief
     * \note
     * \retval
     */
    Interface();

    /**
     * \brief
     * \note
     * \param  command:
     * \retval
     */
    std::string& Query(const std::string &command) const noexcept;

    /**
     * \brief 
     * 
     * \param command
     * \return DriverError 
     */
    void Send(const std::string& command) const noexcept;

    /**
     * \brief 
     * 
     * \param output
     * \return DriverError 
     */
    void Recv(std::string& output) const noexcept;

    /**
     * \brief 
     * 
     * \return DriverError 
     */
    void Recv();


    /**
     * \brief Get the Out String object
     * 
     * \return std::string& 
     */
    char* GetOutString() { return outbuffer.get(); }

    /**
     * \brief Set the Parameter object
     * 
     */
    void SetParameter();

    /**
    * \brief Error Codes for the Driver Interface
    */
    enum Error : uint32_t {

        FAILED = 1,               ///< The Command Failed
        PARAM = 1 << 1,           ///< Bad Paramaters
        PARAMSET = 1 << 2,        ///< Bad Parameter Sets
        STATUS = 1 << 3,          ///< Driver is in a Bad State
        USB_INIT = 1 << 4,        ///< The USB Wasn't Initialized
        USB_RECEIVE = 1 << 5,     ///< The USB Recieved Bad Data
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
    static inline constexpr const char* GetErrorMessage(Error error) noexcept;

private:

    #ifdef _WIN32
    f_dllfunction asic_call_parse;      ///< Function pointer from the DLL
    HINSTANCE dll;                      ///< Function Handle for the DLL
    #endif                              ///< DLL Handle

    std::unique_ptr<char> outbuffer;    ///< Data Buffer For the output data, 65kB wide

};

}