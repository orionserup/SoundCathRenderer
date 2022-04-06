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
#include "Constants.hpp"
#include "Parameters.hpp"

#include <Eigen/Dense>

#include <string>

using std::string;
using std::array;

namespace SoundCath {

using Eigen::Vector;
using Eigen::Matrix;

/// Delay Values for a Group
typedef Vector<int8_t, GROUPELEMENTS> GroupDelays;
/// Phase Offsets 
typedef GroupDelays GroupPhases;

/**
 * \brief 
 * 
 * \param os
 * \param rx
 * \return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const GroupDelays& rx);

/// Delays for the Whole Transducer Array
typedef Matrix<int8_t, NUMGROUPS, GROUPELEMENTS> Delays;

/**
 * \brief Prints 
 * 
 * \param os
 * \param rx
 * \return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const Delays& delays);

/**
 * \brief 
 * 
 * \param is
 * \param delays
 * \return std::istream& 
 */
std::istream& operator>>(std::istream& is, Delays& delays);


// Taylor Coefficient Types
/// Taylor Transmission Coefficients
typedef Vector<int16_t, 8> TxCoeffs;
/**
 * \brief 
 * 
 * \param os
 * \param rx
 * \return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const TxCoeffs& rx);

/// Taylor Receiving Coefficients
typedef Vector<int16_t, 10> RxCoeffs;

/**
 * \brief 
 * 
 * \param os
 * \param rx
 * \return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const RxCoeffs& rx);

// Easy Use Types
/// Represents A Group on the ASIC
typedef uint8_t Group;

/// Represent An Element in the Group
typedef uint8_t Elem;

/// Element Typedef between [0..1023]
typedef struct { 
    
    Group group: 6; 
    Elem loc: 4; 

} Element;

/**
 * \brief Container Class for the ASIC functionality
 */
class ASIC {

public:

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
     * \brief  
     * \note   
     * \param  config
     * \retval 
     */
    ASIC(Driver& driver);

    /**
     * \brief Construct a new ASIC object
     * 
     * \param driver 
     * \param config 
     */
    ASIC(Driver& driver, const ASICParams& config);

    /**
     * \brief Set the Config object
     * 
     * \param config 
     */
    void SetConfig(const ASICParams& config) { params = config; }

    /**
     * \brief Get the Error object
     * 
     * \return ASICError 
     */
    ASIC::Error GetError() const;

    // ------------------------- Fire Commands ---------------------- //

    /**
     * \brief Fire the ASIC with the Given Transmission Delays
     * 
     * \param delays: Delays to Transmit with
     */
    void Fire(const Delays& delays);

    /**
     * \brief Fires the Whole ASIC and Receives to a Group
     * 
     * \param group: Group to receive to
     * \param tx: the Element delays to send with
     * \param rx: The Group Delays to Send With
     */
    void Fire(const Group group, const Delays& tx, const GroupDelays& rx);

    /**
     * \brief Fires a Single Element
     * 
     * \param elem: Element to Fire
     */
    void Fire(const Element elem);

    // ------------------------ Fire Group Commands ---------------- //

    /**
     * \brief Fires a Group with the Given Transmission Delays
     * 
     * \param group: Group to Fire
     * \param delay: Delay to Fire With
     */
    void FireGroup(const Group group, const GroupDelays& tx);

    /**
     * \brief Fire a Group with Tx Delays and Receive from the Same Group with the RX Delays
     * 
     * \param group: Group to send and recieve from
     * \param tx: Group Delays for Transmission
     * \param rx: Group Delays for Reception
     */
    void FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx);

    /**
     * \brief Fire From a Group and Receive from a Different group
     * 
     * \param group: Group to Fire From
     * \param tx: Group Delays To Transmit with 
     * \param rx: Group Delays To Recieve with
     * \param output: Group to Recieve from 
     */
    void FireGroup(const Group txgroup, const GroupDelays& tx, const GroupDelays& rx, const Group rxgroup);

    /**
     * \brief Fire a Group and receive With a Dynamic Phase Rx
     * \note Phase Curves must be set before running
     * 
     * \param group: Group to Fire
     * \param tx: Group Delay for Transmission
     * \param rx: Group Delay for Reception
     * \param rxphases: Phases for the Dynamic Rx
     */
    void FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx, const GroupPhases& rxphases);

    // ----------------------- Recieving Commands ---------------------- //

    /**
     * \brief Read the Last Sent Transmission Delays 
     * 
     * \param[out] delays: Delays to write to
     */
    void ReadTXDelays(Delays& delays);

    /**
     * \brief Read the last Reception Delays
     * 
     * \param[out] delays: Delays received
     */
    void ReadRXDelays(Delays& delays);

    /**
     * \brief Read the Last Received Reception Taylor Coefficents 
     * 
     * \param coeffs: The Received Taylor Coefficients
     */
    void ReadRXCoeffs(RxCoeffs& coeffs);

    /**
     * \brief Recieves to a Single Element
     * 
     * \param[in] elem: Element to Receive to
     */
    void RecvElement(const Element elem);

    //--------------------- Micro Beam Forming Functions --------------------//

    /**
     * \brief Add A Beam to the Queue with the Transmission and Reception Coefficients
     * 
     * \param[in] tx: Transmission Coefficients 
     * \param[in] rx: Reception Coefficients
     */
    void QueueBeam(const TxCoeffs& tx, const RxCoeffs& rx);

    /**
     * \brief Add a Beam to The Beam Queue with Delays Rather than Taylor Coefficients
     * 
     * \param[in] tx: Transmission Delays
     * \param[in] rx: Reception Delays
     */
    void QueueBeam(const Delays& tx, const Delays& rx);

    /**
     * \brief Send a Copy of the Last Sent Beam to the Top of the Queue in the FPGA
     * 
     */
    void QueueRepeatBeam();

    /**
     * \brief Send the Beam Queue To the ASIC
     * 
     */
    void FlushBeamQueue();

    /**
     * \brief Get the Number of Beams in the Beam Queue in the FPGA
     * 
     * \return uint32_t: The Number of Beams in the Queue
     */
    uint32_t GetBeamQueueSize();

    /**
     * \brief Clear the Queue of all of the Beams in the FPGA
     * 
     */
    void ClearBeamQueue();

    /**
     * \brief Send a Beam to Be Fired 
     * 
     * \param[in] beaminqueue: Index of the Beam to Fire
     */
    void TriggerBeam(const uint8_t beaminqueue);

    /**
     * \brief Enter a Low Power State For A while, any command wakes it up
     * 
     */
    void Freeze();

    // -------------------------- Utility Functions ------------------------ //

    /**
     * \brief Get the Capacitance of the Group
     * 
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
    float GetElementCapacitance(const Element element);

    /**
     * \brief Get the Serial Num object
     * 
     * \return std::string 
     */
    std::string GetSerialNum();

    /**
     * \brief Get the Band Gap Voltage of the Device
     * 
     * \return double: The Band Gap in Volts
     */
    double GetBandGapV() const;

    /**
     * \brief Runs all of the Built in Tests from Oldelft
     * 
     * \return true: If the Tests All Passed
     * \return false: If one or More Tests Fail
     */
    bool RunTests() const;

    /**
     * \brief Gets the Error Code Message According to the Error Code
     * \note Is Evaluated at Compile time
     * \param code: The Error Code
     * \return const char*: The String Associated with the Error Code 
     */
    static constexpr const char* GetErrorMessage(const Error code) noexcept;

    /**
     * \brief Gets the Physical Location/Index of an Element with regards to its logical location
     * \note Evaluated at Compile Time
     * \param group: The Logical Group
     * \param loc: The logical Element within the group
     * \return Element: The Actual Physical Location
     */
    static constexpr const Element LookupElement(const Group group, const Elem loc) noexcept;

private:

    Driver& driver;     ///< An Instance of the wrapper for the Oldelft API
    string serialnum;   ///< The Serial Number
    ASICParams params;  ///< The parameters of operation, see the docs

    /**
     * \brief Parse the Error Code and throw all of the Errors in the Code
     * 
     */
    void ThrowErrors(const Error error) const;

    /**
     * \brief Write the Parameters in the params object to the ASIC
     * 
     */
    void SetParams();

    // --------------------------- Command Helpers ----------------------- //

    /**
     * \brief Get the Output Capacitance Command object
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* GetOutputCapacitanceCommand() noexcept;

    /**
     * \brief Get the Op Point Command object
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* GetOpPointCommand() noexcept;

    /**
     * \brief Get the Group Capacitance Command object
     * 
     * \param group
     * \return constexpr const char* 
     */
    static constexpr const char* GetGroupCapacitanceCommand(const Group group) noexcept;

    /**
     * \brief Get the Group In L N A Op Point Command object
     * 
     * \param group
     * \return constexpr const char* 
     */
    static constexpr const char* GetGroupInLNAOpPointCommand(const Group group) noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* GetCWCapacitanceCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* RecvSingleElemCommand() noexcept;

    /**
     * \brief Get the Band Gap Command object
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* GetBandGapCommand() noexcept;

    /**
     * \brief Get the Temperature Command object
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* GetTemperatureCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* RecvGroupCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* EchoSingleCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* FireSingleElemCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* FireGroupCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* FireASICCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* EchoASICCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* EchoGroupCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* EchoASICUpdateCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* EchoASICCoeffCommand() noexcept;

    /**
     * \brief Get the Output Capacitance Command object
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* DynamicEchoGroupCommand() noexcept;

    /**
     * \brief Get the Output Capacitance Command object
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* DynamicEchoASICCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* DynamicEchoASICUpdCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* SendTestPulseCommand() noexcept;

    /**
     * \brief 
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* ReadTXDelaysCommand() noexcept;

    /**
     * \brief Get the Output Capacitance Command object
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* ReadRXDelaysCommand() noexcept;

    /**
     * \brief Get the Output Capacitance Command object
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* ConfigureASICCommand() noexcept;

    /**
     * \brief Get the Output Capacitance Command object
     * 
     * \return constexpr const char* 
     */
    static constexpr const char* RunTestsCommand() noexcept;


};


}
