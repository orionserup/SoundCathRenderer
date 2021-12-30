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

            CW ///< Continuous Mode

        };

        /**
         * @brief  Exception Struct for Error handling
         */
        class Exception {

        public:

            Exception(std::string& message) { this->message = message; }
            std::string& GetMessage() { return this->message; }

        private:

            std::string message; //< Message associated with the Exception
 
        };

        /**
         * @brief  
         * @note   
         * @param  driver: 
         * @param  speed: 
         * @retval 
         */
        ASIC(Interface& driver, ClkSpeed speed = LOW);

        /**
         * @brief Gets the Serial Number from the ASIC
         * @note Uses Default Serial Number call
         * @retval string, the serial number in a string form
         */
        std::string GetSerialNum();

        /**
         * @brief  
         * @note   
         * @param  channel: 
         * @retval 
         */
        double GetOutputCapacitance(const uint8_t channel) const;

        /**
         * @brief  
         * @note   
         * @retval 
         */
        double GetBandGapV() const;

        /**
         * @brief  
         * @note   
         * @retval 
         */
        bool RunTests() const;

    private:

        Interface& driver; //< An Instance of the wrapper for the Oldelft API
        ClkSpeed speed;    //< The ASIC Clock Speed
        Mode mode;         //< The ASIC operation Mode

    };


}
