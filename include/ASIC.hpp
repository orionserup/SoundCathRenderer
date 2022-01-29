/**
 * \file ASIC.hpp
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
#include "Controller.hpp"

#include <string>

using std::string;

/**
 * @brief SoundCath Name Space for internal functionality
 */
namespace SoundCath {

/**
 * \brief 
 * 
 */
enum ASICError : uint8_t {

    OK = 0,                 ///< No Error
    UNKNOWN_CMD = 1 << 0,   ///< Unknown Command
    VALID_ERROR = 1 << 1 ,  ///< A Random Error
    CHKSUM_ERROR = 1 << 2,  ///< A Checksum Error
    BUSY = 1 << 3,          ///< The ASIC is Busy
    LOCKED = 1 << 6,        ///< The ASIC is Locked
    EXTERNAL = 1 << 7       ///< An External Error Occured

};

/**
 * \brief 
 * 
 * \param code 
 * \return constexpr const char* 
 */
constexpr const char* GetASICErrorMessage(const ASICError code) {

    switch (code) {

        case OK:
            return "No Error Occurred";
        case UNKNOWN_CMD:
            return "Unknown Command Sent";
        case VALID_ERROR:
            return "Invalid Data Sent";
        case CHKSUM_ERROR:
            return "Checksum Error";
        case BUSY:
            return "ASIC is Busy Try Again Later";
        case LOCKED:
            return "ASIC is in a Locked State";
        case EXTERNAL:
            return "External Error Detected";
        default:
            return "Unknown Error Detected";
    }

}

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
        NORMAL,
        BMODE

    };

    /**
     * @brief  
     * @note   
     * @param  driver: 
     * @param  speed: 
     * @retval 
     */
    ASIC(const Interface& driver, ClkSpeed speed = LOW, Mode mode = NORMAL);

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

    /**
     * \brief Get the Error object
     * 
     * \return ASICError 
     */
    ASICError GetError();

    /**
     * \brief Set the Elem Delays object
     * 
     * \param delays 
     * \return ASICError 
     */
    void Fire(const Delays& delays);
    
    /**
     * \brief 
     * 
     * \param elem 
     */
    void FireSingle(const uint16_t elem);

    /**
     * \brief Set the Group Delay object
     * 
     * \param group 
     * \param delay 
     * \return ASICError 
     */
    void FireGroup(const uint8_t group, const GroupDelays& delay);

    /**
     * \brief Get the Group Capacitance object
     * 
     * \param group 
     * \return array<float, GROUPELEMENTS>& 
     */
    array<float, GROUPELEMENTS>& GetGroupCapacitance(const uint8_t group);
    
    /**
     * \brief Get the Element Capaitance object
     * 
     * \param elem 
     * \return float 
     */
    float GetElementCapaitance(const uint16_t elem);


private:

    Interface& driver; ///< An Instance of the wrapper for the Oldelft API

    ClkSpeed speed;    ///< The ASIC Clock Speed
    Mode mode;         ///< The ASIC operation Mode
    string serialnum;  ///< The Serial Number
    array<float, GROUPELEMENTS> capacitance_pf;   ///< Element Capacitance

    ASICError errorcode; ///< Any Time an Operation is done It fills this value

};


}
