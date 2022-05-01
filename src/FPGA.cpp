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

#include <fmt/format.h>
#include <iostream>

using SoundCath::FPGA;
using SoundCath::FPGAError;
using SoundCath::FPGAParams;

const char* FPGAError::GetErrorMessage(const FPGAError::Code error) noexcept {

    switch(error) {

        case QSPI:
            return "FPGA Error on the Quad SPI Bus\n";
        case CLOCK:
            return "FPGA Error with the Internal Clock\n";
        case FPGA_POW:
            return "FPGA Power Not Properly Initialized\n";
        case HV_POW:
            return "FPGA High Voltage Power Not Configured\n";
        case LV_POW:
            return "FPGA Low Voltage Power Not Configured\n";
        case POW_FAIL:
            return "FPGA Power Failure wwithin the System\n";
        case CMD:
            return "FPGA Invalid FPGA Command\n";
        case TRIGGER:
            return "FPGA Invalid Trigger or Trigger Misconfigured\n";
        case CLKBUSY:
            return "FPGA CLock is Busy\n";
        case OVERFULL:
            return "FPGA Memory Overfilled, Buffer Overfill\n";
        case FRAME_ERROR:
            return "FPGA Memory Frame Error\n";

        default:
            return "Unknown FPGA Error\n";

    }
}

template<FPGAParams params>
FPGAError::Code FPGA<params>::GetError() const {

    std::string response = face.Query("GetAsicError");

    // "GetASICError:RESULT:ASIC Error Status: hh, FPGA Error Status: hhhhhhhh" // h is a hex digit

    std::string digits = response.substr(response.size() - 8);

    uint32_t code = std::stoi(digits, 0, 16); // get the integer value from the hex string

    return (FPGAError::Code)code;

}

template<FPGAParams params>
FPGA<params>::FPGA(Driver& face): face(face) {

    try {

        face.Send("FPGADescription"); // request the description
        this->desc = face.GetResult(); // get the description from what it gives

        face.Send("FPGAVersion"); // request the version
        this->version = face.GetResult(); // get the result from the result string

    }
    catch(const std::exception& e) {

        std::cerr << "Error While Initializing FPGA:: " << e.what();
        exit(1);

    }
}

