/**
 * \file Exception.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 2022-01-29
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#include "Exception.hpp"

using namespace SoundCath;

/**
 * \brief Gets the error Message Cooresponding with the Error at compile time
 */
constexpr const char* GetErrorMessage(Interface::Error error) noexcept{

    switch (error) {

    case FAILED:
        return "Operation Failed \n";
    case PARAM:
        return "Bad Parameter \n";
    case PARAMSET:
        return "Bad Paramater Set \n";
    case STATUS:
        return "Incorrect Status \n";
    case USB_INIT:
        return "USB Not Initialized \n";
    case USB_RECEIVE:
        return "USB Data Not Recieved \n";
    case USB_SEND:
        return "USB Did Not Send the Data \n";
    case NOT_IMPLEMENTED:
        return "Function or Feature Not Implemented \n";
    case CRC:
        return "Internal CRC Error \n";
    case FPGAERROR:
        return "FPGA Error, Ask for Details From the Interface\n";
    case SWINTERNAL:
        return "Internal Error from the SWI Unit \n";
    case ASICERROR:
        return "Error Returned From the ASIC \n";
    default:
        return "Unknown Error Occured \n";

    }
}

/**
 * \brief 
 * 
 * \param code
 * \return constexpr const char* 
 */
constexpr const char* GetASICErrorMessage(const ASICError code) noexcept {

    switch (code) {

        case UNKNOWN_CMD:
            return "Unknown Command Sent";
        case VALID_ERROR:
            return "Invalid Data Sent";
        case CHKSUM_ERROR:
            return "Checksum Error";
        case BUSY:
            return "ASIC is Busy Try Again Later";
        case LOCKED:
            return "ASIC is in a Locked State";
        case EXTERNAL:
            return "External Error Detected";
        default:
            return "Unknown Error Detected";
    }

}

