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

#include "Interface.hpp"

namespace SoundCath {

    /**
     * \brief 
     * 
     */
    class DriverTester {

    public:

        /**
         * \brief 
         * 
         * \return true 
         * \return false 
         */
        bool TestQuery();

        /**
         * \brief 
         * 
         * \return true 
         * \return false 
         */
        bool TestSend();

        /**
         * \brief 
         * 
         * \return true 
         * \return false 
         */
        bool TestRecv();

    private:

        SoundCath::Interface face; ///< The Interface to Test

    };

}