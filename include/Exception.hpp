/**
 * \file Exception.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 2022-01-29
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <exception>

#include "ASIC.hpp"
#include "FPGA.hpp"
#include "Interface.hpp"

namespace SoundCath {

/**
 * \brief An Exception Class for Error Handling ASIC Errors
 * 
 */
class ASICException: public std::exception {

public:

    /**
     * \brief Construct a new ASICException object
     * 
     * \param error: The Error Code to use
     */
    ASICException(ASIC::Error error);

    /**
     * \brief Returns the Error Message
     * 
     * \return const char*: The Error Message
     */
    inline const char* what() const noexcept;

private:

    ASIC::Error error; ///< The Actual Error Code

};

class DriverException: public std::exception {

public:

    /**
     * \brief Construct a new Driver Exception object
     * 
     * \param error: The Error to Give it
     */
    DriverException(const Interface::Error error);
    
    /**
     * \brief Gets the String Associated with the Error
     * 
     * \return const char*: The Error Message
     */
    const char* what() const noexcept;

private:

    Interface::Error error; ///< The Actual Error

};

class ControllerException: public std::exception {




};

}