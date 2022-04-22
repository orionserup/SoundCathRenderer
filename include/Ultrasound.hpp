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
         * \brief Set the Params object
         * 
         */
        void SetParams() const;
        
        /**
         * \brief Set the Param object
         * 
         * \param group
         * \param param
         * \param value
         */
        void SetParam(const char* const group, const char* const param, const auto& value) const ;

    };

}