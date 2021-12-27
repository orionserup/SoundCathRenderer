#pragma once

#include "asic_call_wrapper_dll.h"
#include <cstdint>
#include <string>
#include <array>

/**
 * @brief  Type definition for the dll imported function for the asic call wrapper
 * @note   From Oldelft
 * @param  f_dllfunction:
 * @retval int: error code
 */
typedef int (_stdcall *f_dllfunction)(char *inString, char *outString);

namespace SoundCath
{

    /**
     * @brief
     * @note
     * @retval None
     */
    enum DriverError : uint32_t {

        OK = 0,                   //< No Error
        FAILED = 1,               //<
        PARAM = 1 << 1,           //<
        PARAMSET = 1 << 2,        //<
        STATUS = 1 << 3,          //<
        USB_INIT = 1 << 4,        //<
        USB_RECEIVE = 1 << 5,     //<
        USB_SEND = 1 << 6,        //<
        NOT_IMPLEMENTED = 1 << 7, //<
        CRC = 1 << 8,             //<
        FPGA = 1 << 9,            //<
        SWINTERNAL = 1 << 10,     //<
        ASICERROR = 1 << 16       //<

    };

    /**
     * @brief
     * @note
     * @retval None
     */
    enum InternalError : uint32_t{

        enum DriverError : uint32_t {

    /**
     * @brief Gets the error Message Cooresponding with the Error
     */
    constexpr const char* GetDriverErrorMessage(DriverError CodedError) noexcept;

    /**
     * @brief Gets the Error Message Associated with the given Internal Error Codes
     */
    constexpr const char* GetInternalErrorMessage(InternalError CodedError) noexcept;

    /**
     * @brief  ASIC interfacer class
     * @note   A wrapper for the Oldeft API
     */

    class Interface
    {

        };

        enum InternalError : uint32_t {

            OK = 0,

        };

            /**
         * @brief
         * @note
         * @retval
         */
        Interface();

        /**
         * @brief
         * @note
         * @param  command:
         * @retval
         */
        std::string Query(const std::string &command) const noexcept;

        /**
         * @brief  Recieves the repsonse from the buffer
         * @note   Just a getter for the get outbuffer fiunction
         * @retval string, the reponse
         */
        std::array<char, UINT16_MAX>& GetOutBuffer() { return outBuffer; }

        /**
         * @brief
         * @note
         * @retval
         */
        ~Interface();

    private:

        f_dllfunction asic_call_parse;  //< Function pointer from the DLL
        HINSTANCE dll;                  //< DLL Handle

        std::array<char, UINT16_MAX> outBuffer;     //< Data Buffer For the output data, 65kB wide
    };
}