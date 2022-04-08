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

#include <sstream>

using SoundCath::ASIC;

const char* ASIC::GetErrorMessage(const ASIC::Error code) noexcept {

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

ASIC::ASIC(Driver& driver): driver(driver) {}


ASIC::ASIC(Driver& driver, const Params::ASICParams& config): driver(driver), params(config) {

    //SetParams();

}

void ASIC::SetConfig(const Params::ASICParams& params) {



}

ASIC::Error ASIC::GetError() const {

    std::string resp = driver.Query(GetErrorCommand());
    // Expected: GetASICError:RESULT:ASIC Error Status: 00, FPGA Error Status 00000000
    const static std::string header("ASIC Error Status: ");
    std::string digits = resp.substr(resp.find(header) + header.size(), 2);

    uint8_t error = std::stoi(digits, 0, 16);

    return (ASIC::Error)error;

}

void ASIC::ThrowErrors(const ASIC::Error error) const {

    if(error & UNKNOWN_CMD) throw ASICException(UNKNOWN_CMD);
    if(error & VALID_ERROR) throw ASICException(VALID_ERROR);
    if(error & CHKSUM_ERROR) throw ASICException(CHKSUM_ERROR);
    if(error & BUSY) throw ASICException(BUSY);
    if(error & LOCKED) throw ASICException(LOCKED);
    if(error & EXTERNAL) throw ASICException(EXTERNAL);

}

void ASIC::Fire(const Delays& delays) {

    std::ostringstream command;


}

void ASIC::Fire(const Group group ,const Delays& tx, const GroupDelays& rx) {




}

void ASIC::Fire(const Element elem) {

    std::ostringstream command;
    command << FireSingleElemCommand() << elem.group << ',' << elem.loc;
    driver.Send(command.str());

}


void ASIC::FireGroup(const Group group, const GroupDelays& tx) {

    std::ostringstream command;
    command << FireGroupCommand() << group << ':' << tx;
    driver.Send(command.str());

}


void ASIC::FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx) {

    std::ostringstream command;
    command << FireGroupCommand() << group << ':' << tx;
    driver.Send(command.str());

}


void ASIC::FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx, const Group output) {

    std::ostringstream command;
    command << FireGroupCommand() << group << ':' << tx;
    driver.Send(command.str());

}


void ASIC::FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx, const GroupPhases& rxphases) {

    std::ostringstream command;
    command << FireGroupCommand() << group << ':' << tx;
    driver.Send(command.str());

}


void ASIC::ReadTXDelays(Delays& delays) {

    std::string resp{ driver.Query(ReadTXDelaysCommand()) };   
    std::string delaystr{ resp.substr(resp.find("[ASIC 0]:") + 1)}; // the delay string will be after the ASIC number
    std::istringstream output(driver.Recv()); // convert the output to a stream so that it can be converted to delays
    output >> delays; // convert to delays

}


void ASIC::ReadRXDelays(Delays& delays) {

    std::string resp{ driver.Query(ReadRXDelaysCommand()) };   
    std::string delaystr{ resp.substr(resp.find("[ASIC 0]:") + 1)}; // the delay string will be after the ASIC number
    std::istringstream output(driver.Recv()); // convert the output to a stream so that it can be converted to delays
    output >> delays; // convert to delays and put it into the delay

}

void ASIC::RecvElement(const Element elem) {


}

 
void ASIC::GetGroupCapacitance(const Group group, std::vector<double>& cap_pf) {


}
    

float ASIC::GetElementCapacitance(const Element element) const { 

    return 0.0f;

}


std::string ASIC::GetSerialNum() {

    return this->serialnum;

}

void ASIC::SetSerialNum(const std::string& serialnum) {

    this->serialnum = serialnum;

}


double ASIC::GetBandGapV() const {

    return 0.0f;

}

bool ASIC::RunTests() const {

    return false;

}

// --------------------------- Utility Functions ------------------------ //


std::ostream& SoundCath::operator<<(std::ostream& os, const SoundCath::GroupDelays& rx) {

    return os;

}

std::ostream& SoundCath::operator<<(std::ostream& os, const SoundCath::Delays& delays) {

    return os;

}

std::istream& SoundCath::operator>>(std::istream& is, SoundCath::Delays& delays) {

    return is;

}

std::ostream& SoundCath::operator<<(std::ostream& os, const SoundCath::TxCoeffs& rx) {

    return os;

}

std::ostream& SoundCath::operator<<(std::ostream& os, const SoundCath::RxCoeffs& rx) {

    return os;

}

// -------------------- Command Functions -------------- //

constexpr const char* ASIC::GetOutputCapacitanceCommand() noexcept {

    return "ElementCapacitance:";

}

constexpr const char* ASIC::GetOpPointCommand() noexcept {

    return "OutputOpPoint:";

}

constexpr const char* ASIC::GetGroupCapacitanceCommand(const Group group) noexcept {

    return "OutputCapacitance:";

}

constexpr const char* ASIC::GetGroupInLNAOpPointCommand(const Group group) noexcept {

    return "";

}

constexpr const char* ASIC::GetCWCapacitanceCommand() noexcept {

    return "";

}

constexpr const char* ASIC::RecvSingleElemCommand() noexcept {

    return "";

}

constexpr const char* ASIC::GetBandGapCommand() noexcept {

    return "GetBandGap";

};

constexpr const char* ASIC::GetTemperatureCommand() noexcept{

    return "GetTemperature";

}

constexpr const char* ASIC::RecvGroupCommand() noexcept {

    return "";
}

constexpr const char* ASIC::EchoSingleCommand() noexcept {

    return "";

}

constexpr const char* ASIC::FireSingleElemCommand() noexcept {

    return "";

}

constexpr const char* ASIC::FireGroupCommand() noexcept {

    return "";

}

constexpr const char* ASIC::FireASICCommand() noexcept {

    return "";

}

constexpr const char* ASIC::EchoASICCommand() noexcept {

    return "";

}

constexpr const char* ASIC::EchoGroupCommand() noexcept {

    return "";

}

constexpr const char* ASIC::EchoASICUpdateCommand() noexcept{

    return "";

}

constexpr const char* ASIC::EchoASICCoeffCommand() noexcept {

    return "";

}

constexpr const char* ASIC::DynamicEchoGroupCommand() noexcept {

    return "";

}

constexpr const char* ASIC::DynamicEchoASICCommand() noexcept {

    return "";

}

constexpr const char* ASIC::DynamicEchoASICUpdCommand() noexcept {

    return "";

}

constexpr const char* ASIC::SendTestPulseCommand() noexcept {

    return "";

}

constexpr const char* ASIC::ReadTXDelaysCommand() noexcept {

    return "";

}

constexpr const char* ASIC::ReadRXDelaysCommand() noexcept {

    return "";

}

constexpr const char* ASIC::ConfigureASICCommand() noexcept {

    return "";

}

constexpr const char* ASIC::RunTestsCommand() noexcept {

    return "";

}

constexpr const char* ASIC::GetErrorCommand() noexcept {

    return "ASICError";

}
