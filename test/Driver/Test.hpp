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

#include "Driver.hpp"

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
         * \test 
         * \throws DriverException: If the Driver doesn't work for one reason or another
         * \return true 
         * \return false 
         */
        bool TestSend();

        /**
         * \brief 
         * \test
         * \return true 
         * \return false 
         */
        bool TestRecv();

    private:

        SoundCath::Driver face; ///< The Interface to Test

    };

}