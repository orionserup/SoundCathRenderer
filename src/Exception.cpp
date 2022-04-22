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
using SoundCath::ASICError;

ASICException::ASICException(const ASICError::Code error) {

    assert(error); // if the error code is zero its not an error

    this->error = error;

}

const char* ASICException::what() const noexcept {

    return ASICError::GetErrorMessage(this->error);

}

using SoundCath::DriverException;

DriverException::DriverException(const DriverError::Code error) {

    assert(error); // a zero code isnt an error

    this->error = error;

} 

const char* DriverException::what() const noexcept {

    return DriverError::GetErrorMessage(this->error);

}

using SoundCath::FPGAException;

FPGAException::FPGAException(const FPGAError::Code error) {

    assert(error);

    this->error = error;

}

const char* FPGAException::what() const noexcept {

    return FPGAError::GetErrorMessage(this->error);

}

