/**
 * \file Test.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 2022-01-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "FPGA.hpp"
#include "Parameters.hpp"

namespace SoundCath {

template<FPGAParams params>

class FPGATester {
public:

    


private:

    FPGA<params> fpga; ///< FPGA To Test

};

}