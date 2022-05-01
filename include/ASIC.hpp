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

#include "Driver.hpp"

#include <Eigen/Dense>

#include <string>
#include <vector>
#include <iostream>
#include <array>
namespace SoundCath {

using Eigen::Vector;
using Eigen::Matrix;

/// Delay Values for a Group
typedef std::array<int8_t, 64> GroupDelays;
/// Phase Offsets 
typedef GroupDelays GroupPhases;

/**
 * \brief Formats a Group Delay to A String/Stream 
 * \throws ios_base::failure: If the Data Can't be Formatted
 * \param[out] os: The Output Stream To Send the Formatted Data To 
 * \param[in] rx: The Delays to Parse and Send
 * \return std::ostream&:  A Modified output stream 
 */
std::ostream& operator<<(std::ostream& os, const GroupDelays& rx);

/// Delays for the Whole Transducer Array
typedef std::array<int8_t, 16 * 64> Delays;
/// Phases for Dynamic Curves
typedef Delays Phases;

/**
 * \brief Prints A Delay to a Stream
 * \throws ios_base::failure: If there is an Issue Reading the Value
 * \param[out] os: Output Stream, Could be a string stream, cout, etc..
 * \param[in] rx: Delay to write to the stream
 * \return std::ostream&: A Modified Stream Reference for chaining
 */
std::ostream& operator<<(std::ostream& os, const Delays& delays);

/**
 * \brief Gets a Delay String and Turns It into an Actual Delay Object
 * \throws ios_base::failure: If it can't resolve the Delay String
 * \param[in] is: Input Stream to Get the String from 
 * \param[out] delays: Actual Delay Object to Write to
 * \return std::istream&: A modified input strema reference
 */
std::istream& operator>>(std::istream& is, Delays& delays);

// Taylor Coefficient Types
/// Taylor Transmission Coefficients
typedef std::array<int16_t, 8> TxCoeffs;

/// Taylor Receiving Coefficients
typedef std::array<int16_t, 10> RxCoeffs;


// Easy Use Types
/// Represents A Group on the ASIC
typedef uint8_t Group;

/// Represent An Element in the Group
typedef uint8_t Elem;

/// Element Typedef between [0..1023]
typedef struct { 
    
    Group group: 6; ///< There are 64 Groups
    Elem loc: 4;    ///< There are 16 Channels Per Group

} Element;

struct ASICError {

    /**
    * \brief Error Codes that are For the ASIC
    * 
    */
    enum Code : uint8_t {

        UNKNOWN_CMD = 1 << 0,   ///< Unknown Command
        VALID_ERROR = 1 << 1 ,  ///< A Random Error
        CHKSUM_ERROR = 1 << 2,  ///< A Checksum Error
        BUSY = 1 << 3,          ///< The ASIC is Busy
        LOCKED = 1 << 6,        ///< The ASIC is Locked
        EXTERNAL = 1 << 7       ///< An External Error Occured
    };

    Code error; ///< The Actual Error Code

    /**
     * \brief Gets the Error Code Message According to the Error Code
     * \note Is Evaluated at Compile time
     * \param code: The Error Code
     * \return const char*: The String Associated with the Error Code 
     */
    static const char* GetErrorMessage(const Code code) noexcept;

    /**
     * \brief Parse the Error Code and throw all of the Errors in the Code
     * \throws ASICException: Thats the Whole Point of the Function
     */
    void ThrowErrors(const Code error) const;
};


template<ASICParams params>
class ASIC {

public:

    /**
     * \brief Construct a new ASIC object
     * 
     * \param[in] driver: Reference to an Interface to Talk to the ASIC With 
     */
    ASIC(Driver& driver);

    /**
     * \brief Get the Error Code From the last Operation
     * 
     * \return ASICError: The Error Code of the Last Operation
     */
    ASICError::Code GetError() const;

    // ------------------------- Fire Commands ---------------------- //

    /**
     * \brief Fire the ASIC with the Given Transmission Delays
     * 
     * \param[in] delays: Delays to Transmit with
     */
    void Fire(const Delays& delays);

    /**
     * \brief Fires the Whole ASIC and Receives to a Group
     * 
     * \param[in] group: Group to receive to
     * \param[in] tx: the Element delays to send with
     * \param[in] rx: The Group Delays to Send With
     */
    void Fire(const Group group, const Delays& tx, const GroupDelays& rx);

    /**
     * \brief Fires a Single Element
     * 
     * \param[in] elem: Element to Fire
     */
    void Fire(const Element elem);

    // ------------------------ Fire Group Commands ---------------- //

    /**
     * \brief Fires a Group with the Given Transmission Delays
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[in] group: Group to Fire
     * \param[in] delay: Delay to Fire With
     */
    void FireGroup(const Group group, const GroupDelays& tx);

    /**
     * \brief Fire a Group with Tx Delays and Receive from the Same Group with the RX Delays
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[in] group: Group to send and recieve from
     * \param[in] tx: Group Delays for Transmission
     * \param[in] rx: Group Delays for Reception
     */
    void FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx);

    /**
     * \brief Fire From a Group and Receive from a Different group
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[in] group: Group to Fire From
     * \param[in] tx: Group Delays To Transmit with 
     * \param[in] rx: Group Delays To Recieve with
     * \param[in] output: Group to Recieve from 
     */
    void FireGroup(const Group txgroup, const GroupDelays& tx, const GroupDelays& rx, const Group rxgroup);

    /**
     * \brief Fire a Group and receive With a Dynamic Phase Rx
     * \note Phase Curves must be set before running
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend 
     * \param[in] group: Group to Fire
     * \param[in] tx: Group Delay for Transmission
     * \param[in] rx: Group Delay for Reception
     * \param[in] rxphases: Phases for the Dynamic Rx
     */
    void FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx, const GroupPhases& rxphases);

    // ----------------------- Recieving Commands ---------------------- //

    /**
     * \brief Read the Last Sent Transmission Delays 
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[out] delays: Delays to write to
     */
    void ReadTXDelays(Delays& delays);

    /**
     * \brief Read the last Reception Delays
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[out] delays: Delays received
     */
    void ReadRXDelays(Delays& delays);

    /**
     * \brief Read the Last Received Reception Taylor Coefficients 
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[out] coeffs: The Received Taylor Coefficients
     */
    void ReadRXCoeffs(RxCoeffs& coeffs);

    /**
     * \brief Recieves to a Single Element
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[in] elem: Element to Receive to
     */
    void RecvElement(const Element elem);

    //--------------------- Micro Beam Forming Functions --------------------//

    /**
     * \brief Add A Beam to the Queue with the Transmission and Reception Coefficients
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[in] tx: Transmission Coefficients 
     * \param[in] rx: Reception Coefficients
     */
    void QueueBeam(const TxCoeffs& tx, const RxCoeffs& rx);

    /**
     * \brief Add a Beam to The Beam Queue with Delays Rather than Taylor Coefficients
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[in] tx: Transmission Delays
     * \param[in] rx: Reception Delays
     */
    void QueueBeam(const Delays& tx, const Delays& rx);

    /**
     * \brief Send a Copy of the Last Sent Beam to the Top of the Queue in the FPGA
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     */
    void QueueRepeatBeam();

    /**
     * \brief Send the Beam Queue To the ASIC
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     */
    void FlushBeamQueue();

    /**
     * \brief Get the Number of Beams in the Beam Queue in the FPGA
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \return uint32_t: The Number of Beams in the Queue
     */
    uint32_t GetBeamQueueSize();

    /**
     * \brief Clear the Queue of all of the Beams in the FPGA
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     */
    void ClearBeamQueue();

    /**
     * \brief Send a Beam to Be Fired 
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[in] beaminqueue: Index of the Beam to Fire
     */
    void TriggerBeam(const uint8_t beaminqueue);

    /**
     * \brief Enter a Low Power State For A while, any command wakes it up
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     */
    void Freeze();

    // -------------------------- Utility Functions ------------------------ //

    /**
     * \brief Get the Capacitance of the Group
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[in] group: Group to Check
     * \param[out] cap_pf: The Capacitances in pF
     * \return void
     */
    void GetGroupCapacitance(const Group group, std::vector<double>& cap_pf);
    
    /**
     * \brief Get the Capacitance of a Single Element
     * 
     * \param[in] element: Element to Check
     * \return float: The Capacitance of the Element
     */
    float GetElementCapacitance(const Element element) const;

    /**
     * \brief Get the Serial Num object
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \return std::string 
     */
    std::string GetSerialNum();

    /**
     * \brief Set the Serial Number of the ASIC
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \param[in] serialnum: Serial Number String to Set it To
     */
    void SetSerialNum(const std::string& serialnum);

    /**
     * \brief Get the Band Gap Voltage of the Device
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \return double: The Band Gap in Volts
     */
    double GetBandGapV() const;

    /**
     * \brief Runs all of the Built in Tests from Oldelft
     * \throws ASICException: If there is an Issue with the ASIC
     * \throws DriverException: If there are any Issues at all on the backend
     * \return true: If the Tests All Passed
     * \return false: If one or More Tests Fail
     */
    bool RunTests() const;



private:

    SoundCath::Driver& driver;      ///< An Instance of the wrapper for the Oldelft API
    std::string serialnum;          ///< The Serial Number

};


}
