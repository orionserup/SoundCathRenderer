/**
 * \file ASIC.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */
#include "ASIC.hpp"
#include "Exception.hpp"

#include <string>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/compile.h>

using SoundCath::ASIC;
using SoundCath::ASICError;
using SoundCath::ASICParams;


const char* ASICError::GetErrorMessage(const ASICError::Code code) noexcept {

    switch (code) {

        case UNKNOWN_CMD:
            return "ASIC Error: Unknown Command Sent\n";
        case VALID_ERROR:
            return "ASIC Error: Invalid Data Sent\n";
        case CHKSUM_ERROR:
            return "ASIC Error: Checksum Error\n";
        case BUSY:
            return "ASIC Error: ASIC is Busy Try Again Later\n";
        case LOCKED:
            return "ASIC Error: ASIC is in a Locked State\n";
        case EXTERNAL:
            return "ASIC Error: External Error Detected\n";
        default:
            return "ASIC Error: Unknown Error Detected\n";
    }
}

template<ASICParams params>
ASIC<params>::ASIC(Driver& driver): driver(driver) {}


template<ASICParams params>
ASICError::Code ASIC<params>::GetError() const {

    std::string resp = driver.Query("GetAsicError");
    // Expected: GetASICError:RESULT:ASIC Error Status: 00, FPGA Error Status 00000000
    const static std::string header("ASIC Error Status: ");
    std::string digits = resp.substr(resp.find(header) + header.size(), 2);

    uint8_t error = std::stoi(digits, 0, 16);

    return (ASICError::Code)error;

}

void ASICError::ThrowErrors(const ASICError::Code error) const {

    if(error & UNKNOWN_CMD) throw ASICException(UNKNOWN_CMD);
    if(error & VALID_ERROR) throw ASICException(VALID_ERROR);
    if(error & CHKSUM_ERROR) throw ASICException(CHKSUM_ERROR);
    if(error & BUSY) throw ASICException(BUSY);
    if(error & LOCKED) throw ASICException(LOCKED);
    if(error & EXTERNAL) throw ASICException(EXTERNAL);

}

template<ASICParams params>
void ASIC<params>::Fire(const Delays& delays) {

    static std::string command = fmt::format("FireASIC:{}", delays);
    this->driver.Send(command);

}

template<ASICParams params>
void ASIC<params>::Fire(const Group group ,const Delays& tx, const GroupDelays& rx) {

    static std::string command = fmt::format(FMT_COMPILE("FireASICRecieve:{},{}:{}:{}"), group, group, tx, rx);
    this->driver.Send(command);

}

template<ASICParams params>
void ASIC<params>::Fire(const Element elem) {

    static std::string command = fmt::format(FMT_COMPILE("FireSingleElement:{},{}"), elem.group, elem.loc);
    this->driver.Send(command);

}

template<ASICParams params>
void ASIC<params>::FireGroup(const Group group, const GroupDelays& tx) {

    static std::string command = fmt::format(FMT_COMPILE("FireGroup:{},{}"), group, tx);
    driver.Send(command);

}

template<ASICParams params>
void ASIC<params>::FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx) {

    static std::string command = fmt::format(FMT_COMPILE("{}:{}:{}"), group, tx, rx);
    driver.Send(command);

}

template<ASICParams params>
void ASIC<params>::FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx, const Group output) {

    static std::string command  = fmt::format(FMT_COMPILE("FireGroupRecieve:{0},{0},{}:{}:{}"), group, output, tx, rx);
    driver.Send(command);

}

template<ASICParams params>
void ASIC<params>::FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx, const GroupPhases& rxphases) {

    static std::string command = fmt::format(FMT_COMPILE("FireGroupReceiveDyn:{0},{0},{0}:{}:{},{}"), group, tx, rx, rxphases);
    driver.Send(command);

}

template<ASICParams params>
void ASIC<params>::ReadTXDelays(Delays& delays) {

    static std::string resp = driver.Query("ReadTxDelays");   
    static std::string delaystr = resp.substr(resp.find("[ASIC 0]:") + 1); // the delay string will be after the ASIC number
    std::istringstream output(driver.Recv()); // convert the output to a stream so that it can be converted to delays
    output >> delays; // convert to delays

}

template<ASICParams params>
void ASIC<params>::ReadRXDelays(Delays& delays) {

    std::string resp{ driver.Query("ReadRxDelays") };   
    std::string delaystr{ resp.substr(resp.find("[ASIC 0]:") + 1)}; // the delay string will be after the ASIC number
    std::istringstream output(driver.Recv()); // convert the output to a stream so that it can be converted to delays
    output >> delays; // convert to delays and put it into the delay

}

template<ASICParams params>
void ASIC<params>::RecvElement(const Element elem) {

    (void)elem;

}

template<ASICParams params>
void ASIC<params>::GetGroupCapacitance(const Group group, std::vector<double>& cap_pf) {

    (void)(group);
    (void)cap_pf;

}
    

template<ASICParams params>
float ASIC<params>::GetElementCapacitance(const Element element) const { 

    (void)element;
    return 0.0f;

}

template<ASICParams params>
std::string ASIC<params>::GetSerialNum() {

    return this->serialnum;

}

template<ASICParams params>
void ASIC<params>::SetSerialNum(const std::string& serialnum) {

    this->serialnum = serialnum;

}

template<ASICParams params>
double ASIC<params>::GetBandGapV() const {

    return 0.0f;

}

template<ASICParams params>
bool ASIC<params>::RunTests() const {

    return false;

}


// --------------------------- Utility Functions ------------------------ //


std::ostream& SoundCath::operator<<(std::ostream& os, const SoundCath::Delays& delays) {

    (void)delays;
    return os;

}

std::istream& SoundCath::operator>>(std::istream& is, SoundCath::Delays& delays) {

    (void)delays;
    return is;

}

