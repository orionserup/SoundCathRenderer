/**
 * \file ASIC.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */
#include "ASIC.hpp"

using namespace SoundCath;

constexpr const char* ASIC::GetErrorMessage(const ASIC::Error code) noexcept {

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
