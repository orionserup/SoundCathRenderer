/**
 * \file Interface.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief Contains the Implementation for the Interface Module
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#include "Interface.hpp"
#include "Exception.hpp"

#include <iostream>

using namespace SoundCath;

#ifdef _WIN32

Interface::Interface(): outbuffer(new char[UINT16_MAX]) {

	this->dll = LoadLibrary("../lib/asic_call_wrapper_dll64");
		if (!this->dll) 
            std::cerr << "\tERROR!!  DLL Not Found or Not Accessable\n";

	asic_call_parse = (f_dllfunction)GetProcAddress(this->dll, "asic_call_parse");
	if (!this->asic_call_parse) 
        std::cerr << "\tERROR!!  Could Not Access or Find the ASIC Call Function in the DLL\n";

}

std::string Interface::Query(char* command) const {

	int result = asic_call_parse(command, outbuffer.get());
		
    if (!result)
		throw DriverException((Error)result);

    return std::string(outbuffer.get());

}

Interface::~Interface() {

	FreeLibrary(this->dll);
    outbuffer.release();

}

#endif

/**
 * \brief Gets the error Message Cooresponding with the Error at compile time
 */
constexpr const char* Interface::GetErrorMessage(Error error) noexcept{

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

