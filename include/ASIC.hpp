/**
 * \file ASIC.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief Contains the ASIC Specific Functionality Definition
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#pragma once 

#include "Interface.hpp"
#include "Constants.hpp"

#include <string>

using std::string;
using std::array;

/**
 * \brief SoundCath Name Space for internal functionality
 */
namespace SoundCath {

/// Delay Values for a Group
typedef array<int8_t, GROUPELEMENTS> GroupDelays;

/// Delays for the Whole Transducer Array
typedef array<array<GroupDelays, YGROUPS>, XGROUPS + 1> Delays;

/// Phase Offsets 
typedef Delays Phases;

// Taylor Coefficient Types
/// Taylor Transmission Coefficients
typedef array<int16_t, 8> TxCoeffs;

/// Taylor Receiving Coefficients
typedef array<int16_t, 10> RxCoeffs;

// Easy Use Types
/// Represents A Group on the ASIC
typedef uint8_t Group;

/// Represent An Element in the Group
typedef uint8_t Elem;

/// Element Typedef between [0..1023]
typedef struct { Group group: 6; Elem loc: 4; } Element;

/**
 * \brief Gets the Physical Location/Index of an Element with regards to its logical location
 * \note Evaluated at Compile Time
 * \param group: The Logical Group
 * \param loc: The logical Element within the group
 * \return Element: The Actual Physical Location
 */
constexpr const Element LookupElement(const Group group, const Elem loc) noexcept;

/**
 * \brief Container Class for the ASIC functionality
 */
class ASIC {

public:

    /**
     * \brief Enum for the clock speed of the ASIC, Either Low: 25MHz, or High: 100MHz
     */
    enum ClkSpeed : uint8_t {

        LOW = 25,  ///< 25MHz
        HIGH = 100 ///< 100MHz

    };

    /**
     * \brief 
     * 
     */
    enum BModeSetting : bool {

        DELAYS = 0, ///< Send the Element Delays
        COEFFS = 1  ///< Send the Compression Coefficient

    };

    /**
     * \brief  
     * \note   
     * \param  driver: 
     * \param  speed: 
     * \retval 
     */
    ASIC(const Interface& driver, ClkSpeed speed = LOW);

    /**
     * \brief Get the Error object
     * 
     * \return ASICError 
     */
    void GetError();

    // ------------------------- Fire Commands ---------------------- //

    /**
     * \brief 
     * 
     * \param delays
     */
    void Fire(const Delays& delays);

    /**
     * \brief 
     * 
     * \param group
     * \param tx
     * \param rx
     */
    void Fire(const Group group, const Delays& tx, const GroupDelays& rx);
    
    // --------------------- Fire Element Commands ----------------- //

    /**
     * \brief 
     * 
     * \param elem 
     */
    void FireElement(const Element elem);

    // ------------------------ Fire Group Commands ---------------- //

    /**
     * \brief Set the Group Delay object
     * 
     * \param group 
     * \param delay 
     * \return ASICError 
     */
    void FireGroup(const Group group, const GroupDelays& tx);

    /**
     * \brief 
     * 
     * \param group
     * \param tx
     * \param rx
     */
    void FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx);

    /**
     * \brief 
     * 
     * \param group
     * \param tx
     * \param rx
     * \param output
     */
    void FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx, const Group output);

    /**
     * \brief 
     * 
     * \param group
     * \param tx
     * \param rx
     * \param rxphases
     */
    void FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx, const Phases& rxphases);

    // ----------------------- Recieving Commands ---------------------- //

    /**
     * \brief 
     * 
     * \param delays
     */
    void ReadTXDelays(Delays& delays);

    /**
     * \brief 
     * 
     * \param delays
     */
    void ReadRXDelays(Delays& delays);

    /**
     * \brief 
     * 
     * \param coeffs
     */
    void ReadRXCoeffs(RxCoeffs& coeffs);

    /**
     * \brief 
     * 
     * \param group
     * \param elem
     * \param channe1
     */
    void RecvElement(const Element elem);

    // -------------------------- Utility Functions ------------------------ //

    /**
     * \brief Get the Group Capacitance object
     * 
     * \param group 
     * \return array<float, GROUPELEMENTS>& 
     */
    array<float, GROUPELEMENTS> GetGroupCapacitance(const uint8_t group);
    
    /**
     * \brief Get the Element Capaitance object
     * 
     * \param element
     * \return float 
     */
    float GetElementCapacitance(const Element element);

    /**
     * \brief Gets the Serial Number from the ASIC
     * \note Uses Default Serial Number call
     * \retval string, the serial number in a string form
     */
    std::string GetSerialNum();

    /**
     * \brief  
     * \note   
     * \retval 
     */
    double GetBandGapV() const;

    /**
     * \brief  
     * \note   
     * \retval 
     */
    bool RunTests() const;

    /**
     * \brief Error Codes that are For the ASIC
     * 
     */
    enum Error : uint8_t {

        UNKNOWN_CMD = 1 << 0,   ///< Unknown Command
        VALID_ERROR = 1 << 1 ,  ///< A Random Error
        CHKSUM_ERROR = 1 << 2,  ///< A Checksum Error
        BUSY = 1 << 3,          ///< The ASIC is Busy
        LOCKED = 1 << 6,        ///< The ASIC is Locked
        EXTERNAL = 1 << 7       ///< An External Error Occured

    };

    /**
     * \brief Gets the Error Code Message According to the Error Code
     * \note Is Evaluated at Compile time
     * \param code: The Error Code
     * \return const char*: The String Associated with the Error Code 
     */
    static constexpr const char* GetErrorMessage(const Error code) noexcept;

private:

    /**
     * \brief 
     * 
     * \param setting
     * \return ASICError 
     */
    void BModeInit(BModeSetting setting);
    
    /**
     * \brief 
     * 
     * \return ASICError 
     */
    void CWModeInit();

    Interface& driver; ///< An Instance of the wrapper for the Oldelft API

    ClkSpeed speed;    ///< The ASIC Clock Speed
    string serialnum;  ///< The Serial Number

};


}
