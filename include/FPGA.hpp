/**
 * \file FPGA.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-28-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "Interface.hpp"
#include <string>

using std::string;

namespace SoundCath {

enum HardwareID : uint8_t  {

    TestBoard = 0,
    HandleBoard = 1

};

enum FPGAError : uint8_t {

    

};

class FPGA {

public:

    /**
     * \brief Construct a new FPGA object
     * 
     * \param face 
     */
    FPGA(Interface& face);

    /**
     * \brief Destroy the FPGA object
     * 
     */
    ~FPGA();

    /**
     * \brief Get the Version object
     * 
     * \return string 
     */
    string GetVersion();

    /**
     * \brief 
     * 
     * \return HardwareID 
     */
    HardwareID GetID();

    /**
     * \brief Get the Description object
     * 
     * \return string 
     */
    string GetDescription();




private:

    Interface& face;
    string version;
    HardwareID id;
    string desc;
    
    FPGAError error;


};

}
