/**
 * \file Ultrasound.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "Controller.hpp"
#include "ASIC.hpp"
#include "FPGA.hpp"
#include "Driver.hpp"
namespace SoundCath {
    
    template<USParams params>
    class UltraSound {

    public:

        UltraSound();


    private:

        Controller<params.conparams, params.trparams> controller;
        ASIC<params.asicparams> asic;
        FPGA<params.fpgaparams> fpga;
        Driver driver;

        /**
         * \brief Set the Params Given in the Template Params 
         * 
         */
        void SetParams() const;
        
        /**
         * \brief Set the Param object
         * 
         * \param[in] group: The Group the Parameter is In
         * \param[in] param: The Parameter Name
         * \param[in] value: The Value To Set it to, will be converted to a String
         */
        void SetParam(const char* const group, const char* const param, const auto& value) const ;

    };

}