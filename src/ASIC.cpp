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

    std::string resp = driver.Query(GetErrorCommand());
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

    std::ostringstream command;


}

template<ASICParams params>
void ASIC<params>::Fire(const Group group ,const Delays& tx, const GroupDelays& rx) {




}

template<ASICParams params>
void ASIC<params>::Fire(const Element elem) {

    std::ostringstream command;
    command << FireSingleElemCommand() << elem.group << ',' << elem.loc;
    driver.Send(command.str());

}

template<ASICParams params>
void ASIC<params>::FireGroup(const Group group, const GroupDelays& tx) {

    std::ostringstream command;
    command << FireGroupCommand() << group << ':' << tx;
    driver.Send(command.str());

}

template<ASICParams params>
void ASIC<params>::FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx) {

    std::ostringstream command;
    command << FireGroupCommand() << group << ':' << tx;
    driver.Send(command.str());

}

template<ASICParams params>
void ASIC<params>::FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx, const Group output) {

    std::ostringstream command;
    command << FireGroupCommand() << group << ':' << tx;
    driver.Send(command.str());

}

template<ASICParams params>
void ASIC<params>::FireGroup(const Group group, const GroupDelays& tx, const GroupDelays& rx, const GroupPhases& rxphases) {

    std::ostringstream command;
    command << FireGroupCommand() << group << ':' << tx;
    driver.Send(command.str());

}

template<ASICParams params>
void ASIC<params>::ReadTXDelays(Delays& delays) {

    std::string resp{ driver.Query(ReadTXDelaysCommand()) };   
    std::string delaystr{ resp.substr(resp.find("[ASIC 0]:") + 1)}; // the delay string will be after the ASIC number
    std::istringstream output(driver.Recv()); // convert the output to a stream so that it can be converted to delays
    output >> delays; // convert to delays

}

template<ASICParams params>
void ASIC<params>::ReadRXDelays(Delays& delays) {

    std::string resp{ driver.Query(ReadRXDelaysCommand()) };   
    std::string delaystr{ resp.substr(resp.find("[ASIC 0]:") + 1)}; // the delay string will be after the ASIC number
    std::istringstream output(driver.Recv()); // convert the output to a stream so that it can be converted to delays
    output >> delays; // convert to delays and put it into the delay

}

template<ASICParams params>
void ASIC<params>::RecvElement(const Element elem) {


}

template<ASICParams params>
void ASIC<params>::GetGroupCapacitance(const Group group, std::vector<double>& cap_pf) {


}
    

template<ASICParams params>
float ASIC<params>::GetElementCapacitance(const Element element) const { 

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

template<ASICParams params>
constexpr const char* ASIC<params>::GetOutputCapacitanceCommand() noexcept {

    return "ElementCapacitance:";

}

template<ASICParams params>
constexpr const char* ASIC<params>::GetOpPointCommand() noexcept {

    return "OutputOpPoint:";

}

template<ASICParams params>
constexpr const char* ASIC<params>::GetGroupCapacitanceCommand(const Group group) noexcept {

    return "OutputCapacitance:";

}

template<ASICParams params>
constexpr const char* ASIC<params>::GetGroupInLNAOpPointCommand(const Group group) noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::GetCWCapacitanceCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::RecvSingleElemCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::GetBandGapCommand() noexcept {

    return "GetBandGap";

};

template<ASICParams params>
constexpr const char* ASIC<params>::GetTemperatureCommand() noexcept{

    return "GetTemperature";

}

template<ASICParams params>
constexpr const char* ASIC<params>::RecvGroupCommand() noexcept {

    return "";
}

template<ASICParams params>
constexpr const char* ASIC<params>::EchoSingleCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::FireSingleElemCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::FireGroupCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::FireASICCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::EchoASICCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::EchoGroupCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::EchoASICUpdateCommand() noexcept{

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::EchoASICCoeffCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::DynamicEchoGroupCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::DynamicEchoASICCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::DynamicEchoASICUpdCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::SendTestPulseCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::ReadTXDelaysCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::ReadRXDelaysCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::ConfigureASICCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::RunTestsCommand() noexcept {

    return "";

}

template<ASICParams params>
constexpr const char* ASIC<params>::GetErrorCommand() noexcept {

    return "ASICError";

}
