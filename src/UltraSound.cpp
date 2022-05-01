/**
 * \file UltraSound.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 2022-04-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Ultrasound.hpp"

#include "fmt/format.h"
#include "fmt/compile.h"

using SoundCath::UltraSound;
using SoundCath::USParams;

template<USParams params>
UltraSound<params>::UltraSound(): driver(), asic(driver), fpga(driver) {

    SetParams();

}

template<USParams params>
void UltraSound<params>::SetParams() const {
    
    const char* group = "Config";
    SetParam(group, "TxClkDivider", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "TxDelayLSBOpt", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "TxDelayLSBRand", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "RxAssignmentOffset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "RxDelayLSBOpt", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "RxDelayLSBRand", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "BeamContRx", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "BeamContTx", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "RandomizeDynUpdate", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "ErrorStopMask", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "LockDuringBeam", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "IBiasCal", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "AsicEnabled", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);

    group = "ConfigCore";
    SetParam(group,"ISelLNA", params.asicparams.core.ISelLNA);
    SetParam(group, "ISelOdrv", params.asicparams.core.ISelOdrv);
    SetParam(group, "ISelResCntl", params.asicparams.core.ISelResCtrl);
    SetParam(group, "ISelDcGND", params.asicparams.core.ISelDcGND);
    SetParam(group, "ISelPpHVP", params.asicparams.core.ISelLNA);
    SetParam(group, "ResCal", params.asicparams.core.ResCal);
    //SetParam(group, "ResCntlOverwrite", params.asicparams.core.ISelLNA));
    SetParam(group, "AnalogResetNoRx", params.asicparams.core.AnalogResetNoRx);
    SetParam(group, "AnalogResetAuto", params.asicparams.core.AnalogResetAuto);
    SetParam(group, "RxAlwaysEn", params.asicparams.core.RxAlwaysEn);
    //SetParam(group, "TxPrechargeRandomIsrc", params.asicparams.core.ISelLNA));
    SetParam(group, "LNAAutoPowerDown", params.asicparams.core.LNAAutoPowerDown);

    group = "XmitWF";
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    
    group = "BeamTiming";
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    
    group = "BModeSendBeam";
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "UseAnalogReset", params.asicparams.core.LNAAutoPowerDown);
    

    group = "ConfigDRV";
    SetParam(group, "BiasSel", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "FFen", params.asicparams.core.LNAAutoPowerDown);
    SetParam(group, "Enable", params.asicparams.core.LNAAutoPowerDown);

}

template<USParams params>
void UltraSound<params>::SetParam(const char* const group, const char* const param, const auto& value) const {

    driver.Send(fmt::format(FMT_COMPILE("SetParam:{},{}:{}"), group, param, value));

}