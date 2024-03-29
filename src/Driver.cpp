/**
 * \file Driver.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief Contains the Implementation for the Interface Module
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#include "Driver.hpp"
#include "Exception.hpp"

#include <fmt/format.h>
#include <plog/Log.h>

#include <iostream>

using SoundCath::Driver;
using SoundCath::DriverException;

static const char* const TAG = "Driver::";

#if defined(_WIN32) || defined(_WIN64)
#define DLL "..\\..\\lib\\asic_call_wrapper_dll64.dll"

Driver::Driver() {

	this->dll = LoadLibrary(DLL);
	if (!this->dll) { 
        PLOGE << TAG << "DLL Not Found or Not Accessable\n";
        exit(1);
    }

	asic_call_parse = (f_dllfunction)GetProcAddress(this->dll, "asic_call_parse");
	if (!this->asic_call_parse){ 
        PLOGE << TAG << "Could Not Access or Find the ASIC Call Function in the DLL\n";
        exit(1);
    }
}

void Driver::Send(const std::string& command) const {

    PLOGD << TAG << "Sending: " << command << '\n';
    int result = asic_call_parse((char*)command.c_str(), (char*)outbuffer.data());
		
    if (result) {

        DriverError::ThrowErrors((DriverError::Code)result);
        PLOGE << fmt::format("{} Received Error Code from DLL {}\n", TAG, result);

    }
}

Driver::~Driver() {

    Send(CloseCommand());
	FreeLibrary(this->dll);

}

#else

Driver::~Driver() {}

void Driver::Send(const std::string& command) const {

    std::cout << command;

}
/* The end of the ifdef statement. */

#endif

using SoundCath::DriverError;

/**
 * \brief Gets the error Message Cooresponding with the Error at compile time
 */
const char* DriverError::GetErrorMessage(DriverError::Code error) noexcept{

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

void DriverError::ThrowErrors(const DriverError::Code error) {

    if(error & FAILED) throw DriverException(FAILED);
    if(error & PARAM) throw DriverException(PARAM);
    if(error & PARAMSET) throw DriverException(PARAMSET);
    if(error & STATUS) throw DriverException(STATUS);
    if(error & USB_INIT) throw DriverException(USB_INIT);
    if(error & USB_RECEIVE) throw DriverException(USB_RECEIVE);
    if(error & USB_SEND) throw DriverException(USB_SEND);
    if(error & NOT_IMPLEMENTED) throw DriverException(NOT_IMPLEMENTED);
    if(error & CRC) throw DriverException(CRC);
    if(error & FPGAERROR) throw DriverException(FPGAERROR);
    if(error & SWINTERNAL) throw DriverException(SWINTERNAL);
    if(error & ASICERROR) throw DriverException(ASICERROR);

}

std::string Driver::Query(const std::string& command) const {

    Send(command);
    return Recv();

}

void Driver::Recv(std::string& output) const noexcept {

    output = GetOutString();
    PLOGD << fmt::format("{} Received: {} from Device", TAG, output);

}

std::string Driver::GetResult() const noexcept {

    std::string output(GetOutString());
    const std::string resultflag("RESULT:");
    size_t pos = output.find(resultflag);
    
    if(pos == std::string::npos) { // if we cant find "RESULT:"
        pos = output.find_last_of(':'); // find the last colon 
        if(pos == std::string::npos) // if we can't find that 
            return output;  // return the original string
        return output.substr(pos + 1); // return the string past the colon
    }

    return output.substr(pos + resultflag.size()); // return the string past the result:

}

