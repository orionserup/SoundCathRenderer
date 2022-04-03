/**
 * \file Constants.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief Contains the Device Specific Physical and Logical Constants
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#pragma once

#define XGROUP_PITCH_UM 4*1.80
#define YGROUP_PITCH_UM XGROUP_PITCH_UM

#define XPITCH_UM XGROUP_PITCH_UM/4
#define YPITCH XPITCH_UM

#define TXDELAYRES_NS 12.5
#define RXDELAYRES_NS 20

#define L1_FACTOR 4
#define L2_FACTOR 64
#define L3_FACTOR 1500
#define L4_FACTOR_SQ 32

#define ELEMENT_DELAY_FP 3
#define CENTER_DELAY_FP 3

#define XGROUPS 16
#define YGROUPS 4

#define NUMGROUPS 64
#define NUMELEMENTS 1024

#define ALLGROUPS 255

#define GROUPELEMENTS 16

#define TX_ELEMENTS 16
#define RX_ELEMENTS 16

#define SOUND_SPEED_PHANTOM 1540.0f
#define SOUND_SPEED_WATER 1490.f