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
#include <cassert>

using SoundCath::ControllerException;

ControllerException::ControllerException(const char* message) {

    assert(message);

    this->message = message;

}

const char* ControllerException::what() const noexcept {

    return this->message;

}

using SoundCath::ASICException;

ASICException::ASICException(const ASIC::Error error) {

    assert(error); // if the error code is zero its not an error

    this->error = error;

}

const char* ASICException::what() const noexcept {

    return ASIC::GetErrorMessage(this->error);

}

using SoundCath::DriverException;

DriverException::DriverException(const Driver::Error error) {

    assert(error); // a zero code isnt an error

    this->error = error;

} 

const char* DriverException::what() const noexcept {

    return Driver::GetErrorMessage(this->error);

}

using SoundCath::FPGAException;

FPGAException::FPGAException(const FPGA::Error error) {

    assert(error);

    this->error = error;

}

const char* FPGAException::what() const noexcept {

    return FPGA::GetErrorMessage(this->error);

}

