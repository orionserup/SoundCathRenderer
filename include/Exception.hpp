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
     * \param[in] error: The Error Code to use
     */
    ASICException(const ASICError::Code error);

    virtual ~ASICException(){}

    /**
     * \brief Returns the Error Message
     * 
     * \return const char*: The Error Message
     */
    const char* what() const noexcept override;

private:

    
    ASICError::Code error; ///< The Actual Error Code

};

/**
 * \brief 
 * 
 */
class DriverException: public std::exception {

public:

    /**
     * \brief Construct a new Driver Exception object
     * 
     * \param error: The Error to Give it
     */
    DriverException(const DriverError::Code error);
    
    virtual ~DriverException() {}

    /**
     * \brief Gets the String Associated with the Error
     * 
     * \return const char*: The Error Message
     */
    const char* what() const noexcept override;

private:

    DriverError::Code error; ///< The Actual Error

};

/**
 * \brief 
 * 
 */
class ControllerException: public std::exception {

public:

    /**
     * \brief Construct a new Controller Exception object
     * 
     * \param message
     */
    ControllerException(const char* message);

    virtual ~ControllerException() {}

    /**
     * \brief 
     * 
     * \return const char* 
     */
    const char* what() const noexcept override;

private:

    const char* message;    ///< The Error Message

};

class FPGAException: public std::exception {

public:

    /**
     * \brief Construct a new FPGAException object
     * 
     * \param[in] error
     */
    FPGAException(const FPGAError::Code error);

    virtual ~FPGAException() {}

    /**
     * \brief 
     * 
     * \return const char* 
     */
    const char* what() const noexcept override;

private:

    FPGAError::Code error;  ///< 

};

}