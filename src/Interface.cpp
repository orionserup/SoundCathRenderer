#include "Interface.hpp"
#include <iostream>

SoundCath::Interface::Interface() {

	try {

		this->dll = LoadLibrary("../lib/asic_call_wrapper_dll64.dll");
		if (!this->dll) throw "DLL Not Found or Not Accessable\n";

		this->asic_call_parse = (f_dllfunction)GetProcAddress(this->dll, "asic_call_parse");
		if (!this->asic_call_parse) throw "Could Not Access or Find the ASIC Call Function in the DLL\n";

	}
	catch (std::string& error) {

		std::cerr << "ERROR!! :: " << error;
		exit(1);

	}

}

std::string& SoundCath::Interface::Call(const std::string& command) {

	try {

		int result = asic_call_parse(command.c_str(), outBuffer);
		if (!result) throw result;

	}
	catch (std::string& error) {

		std::cerr << "Error "
	}

}

SoundCath::Interface::~Interface() {

	FreeLibrary(this->dll);

}