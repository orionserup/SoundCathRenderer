/**
 * \file FPGA.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief Contains the Implementation of the FPGA Class
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */
#include "FPGA.hpp"

using namespace SoundCath;

constexpr const char* FPGA::GetErrorMessage(const FPGA::Error error) noexcept {

    switch(error) {

        case QSPI:
            return "Error on the Quad SPI Bus";
        case CLOCK:
            return "Error with the Internal Clock";
        case FPGA_POW:
            return "FPGA Power Not Properly Initialized";
        case HV_POW:
            return "High Voltage Power Not Configured";
        case LV_POW:
            return "Low Voltage Power Not Configured";
        case POW_FAIL:
            return "Power Failure wwithin the System";
        case CMD:
            return "Invalid FPGA Command";
        case TRIGGER:
            return "Invalid Trigger ot Trigger Misconfigured";
        case CLKBUSY:
            return "FPGA CLock is Busy";
        case OVERFULL:
            return "FPGA Memory Overfilled, Buffer Overfill";
        case FRAME_ERROR:
            return "FPGA Memory Frame Error";

        default:
            return "Unknown FPGA Error";

    }
}