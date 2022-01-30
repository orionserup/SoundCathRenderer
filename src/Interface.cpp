/**
 * \file Interface.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#include "Interface.hpp"
#include <iostream>

using namespace SoundCath;

#ifdef _WIN32

Interface::Interface() {

	this->dll = LoadLibrary("../lib/asic_call_wrapper_dll64");
		if (!this->dll) 
            std::cerr << "\tERROR!!  DLL Not Found or Not Accessable\n";

	asic_call_parse = (f_dllfunction)GetProcAddress(this->dll, "asic_call_parse");
	if (!this->asic_call_parse) 
        std::cerr << "\tERROR!!  Could Not Access or Find the ASIC Call Function in the DLL\n";

}

std::string Interface::Query(const std::string& command) const noexcept {

	int result = asic_call_parse((char*)command.c_str(), (char*)outBuffer.data());
		
    if (!result)
		std::cerr << "\tError!! " << GetDriverErrorMessage((DriverError)result);

    return std::string((char*)outBuffer.data());

}

Interface::~Interface() {

	FreeLibrary(this->dll);

}

#endif