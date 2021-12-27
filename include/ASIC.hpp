/**
* @file ASIC.hpp
* @authors Orion Serup (orion@soundcath.com)
*/

#pragma once 

#include "main.hpp"
#include "Interface.hpp"

#include <string>

/**
 * @brief SoundCath Name Space for internal functionality
 */
namespace SoundCath {

    /**
     * @brief Container Class for the ASIC functionality
     */
    class ASIC {

    public:

        /**
         * @brief Enum for the clock speed of the ASIC, Either Low: 25MHz, or High: 100MHz
         */
        enum ClkSpeed : uint8_t {

            LOW = 25,  ///< 25MHz
            HIGH = 100 ///< 100MHz

        };

        /**
         * @brief Mode for the ASIC
         */
        enum Mode : uint8_t {

            CW, ///< Continuous Mode

        };

        /**
         * @brief  Exception Struct for Error handling
         */
        struct Exception {
 
        };

        /**
         * @brief Default Constructor (25MHz and a Default Driver
         */
        ASIC() = default;

        /**
         * @brief  Constructor for ASIC where the driver is the argument
         * @note   The default speed is used (25MHz)
         * @param  driver: Reference to an existing ASIC Interface Object 
         * @retval ASIC Instance 
         */
        ASIC(Interface& driver);

        /**
         * @brief  
         * @note   
         * @param  speed: 
         * @retval 
         */
        ASIC(ClkSpeed speed);

        /**
         * @brief  
         * @note   
         * @param  driver: 
         * @param  speed: 
         * @retval 
         */
        ASIC(Interface& driver, ClkSpeed speed);

        /**
         * @brief  
         * @note   
         * @param  channel: 
         * @retval 
         */
        double GetOutputCapacitance(const uint8_t channel);

        /**
         * @brief  
         * @note   
         * @retval 
         */
        double GetBandGapV();

        /**
         * @brief  
         * @note   
         * @retval 
         */
        bool RunTests();

        /**
         * @brief  
         * @note   
         * @retval 
         */
        ~ASIC();

    private:

        Interface driver;

    };


}
