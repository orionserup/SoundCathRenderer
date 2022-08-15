/**
 * \file Parameters.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief Contains the Parameter Structures and the Default values For the Parameters
 * \version 0.1
 * \date 2022-04-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <string_view>

namespace SoundCath {

    /// \todo Add a Compile Time JSON Parameter Parser so the Parameters can be worked with much more easily, Jason Turner's JSON Library should do the trick

    /// Parameters for the FPGA, see the Docs for the complete list of params
    struct FPGAParams {

        const char* const Name{nullptr}; ///< The name of the FPGA/Driver Device
        bool DisableLED{true};  ///< Whether or not to have the LED on

    };

    /// Parameters for the ASIC
    struct ASICParams {

        /// Settings in the Core Group, directly from the PDF Documentation
        struct CoreSettings {

            uint8_t ISelLNA{4};             ///< Current Sensing LNA (0 - 15) Default 4
            uint8_t ISelOdrv{3};            ///< Current Setting Output Driver (0 - 15) Default 3
            uint8_t ISelResCtrl{6};         ///< Current Sensing TGC (0 - 15) Default 6
            uint8_t ISelDcGND{8};           ///< Current Settting Discharge to Ground (0 - 15) Default 8
            uint8_t ResCal{4};              ///< LNA Bias Resistor Calibration Current (0 - 7) Default 4
            bool AnalogResetNoRx{false};    ///< Automatic Analog Reset When Not in RX (t/f) Default false
            bool AnalogResetAuto{true};     ///< Automatic Transmission/Reception Control (t/f) Default true
            bool LNAAutoPowerDown{true};    ///< Automatic LNA Powerdown when not in RX (t/f) Default true
            bool BiasEn{true};              ///< Enable Biasing (t/f) Default true
            bool ODrvEn{true};              ///< Enable Overdrive (t/f) Default true
            bool RxAlwaysEn{true};          ///< Always Enable Reception (t/f) Default true
            bool CWEn{false};               ///< Enable Continuous Mode (t/f) Default True
            bool LNAEn{true};               ///< Enable LNA (t/f) Default true

        };

        /// The Internal Clock Speed of the ASIC
        enum ClkSpeed : uint8_t {

            LOW = 25,  ///< 25MHz
            HIGH = 100 ///< 100MHz

        };

        /// Settings having to do with the Internal Beam Forming and Otherwise, see the documentation for the ASIC for more details
        struct BeamTiming {

            uint8_t SetupTime{25};       ///<           
            uint8_t RunRxTime{30};       ///<
            uint8_t RunTXTime{40};       ///<
            uint8_t StopTXTime{30};      ///<
            uint8_t StopRXTime{20};      ///<
            uint8_t AnaResetStopTime{20};///<
            uint8_t PreChargeTime{30};   ///<

        };


        struct DRVConfig {

            bool Enable{true};    ///< If DRV is Enabled
            bool FFen{true};      ///<
            uint8_t Bias{7};   ///<

        };

        /// Settings for the uBeamforming mode
        struct BModeSettings {

            std::array<bool, 64> offgroups{};   ///< The beam forming groups that are inactive
            bool usequeue{true};                ///< Whether or not to queue up the beams before sending
            float depthrange{.250f};            ///< The Range of the Depth
            float dbrange{40.0f};               ///< The Power Range

        };

        CoreSettings core;          ///< The Core Settings
        BModeSettings bmodesettings;///< uBeanforming Settings 
        ClkSpeed speed{LOW};        ///< ASIC Clock Speed
        DRVConfig drvconfig;        ///< DRV Configuration
        BeamTiming timing;          ///< The Beam Timing

    };

    struct GUIParams {


    };

    struct RenderParams {

        float fps{20.0f};   ///< The Rendering Frames Per Second

    };

    struct TransducerParams {

        double pitch_nm{180000.0};       ///< The Element Pitch in the X and Y Direction
        double group_pitch_nm{720000.0}; ///< The Pitch of the Groups in the X and Y Direction
        double soundspeed{1490.0};  ///< The Speed of Sound
        uint8_t numgroups{64};      ///< The Number of Groups
        uint8_t ygroups{16};        ///< The Number of Groups in the Y Direction
        uint8_t xgroups{4};         ///< The Number of Groups  in the X Direction
        uint8_t xelems{4};          ///< The Number of Elements In Each Group in the X Direction
        uint8_t yelems{4};          ///< The Number of Elements in Each group in the Y Direction
        uint8_t elempergroup{16};   ///< The Number of Elements in each Group
        uint16_t numelements{1024}; ///< The Number of Elements on the Transducer

    };

    struct ControllerParams {

        struct RxParams {

            // Parameters for Standard Delay and Taylor Coeff Calculations
            uint8_t L0{128};        ///< The First Term Lookup Table Correction Factor       
            uint16_t L1{1024};      ///< The Second Term Lookup Table Correction Factor
            uint16_t L2{8192};      ///< The Third Term Lookup Table Correction Factor
            uint8_t L1MAX{6};       ///< The Maximum Value for the Second Term
            uint8_t L2MAX{1};       ///< The Maximum Value for the Third Term
            uint8_t c78factor{16};  ///< Correction Factor for the Seventh and Eigth Coeff
            
            // Dynamic Curve Parameters
            double start_depth_m{.00001};   ///< The Depth to Start Scanning At
            double stop_depth_m{.0001};     ///< The Depth to End Scanning At
            double delay_res_ns{20.0};      ///< The Resolution of the ASIC in Nano Seconds

        };

        struct TxParams {

            double xmax{7.5};
            double ymax{1.5};
            uint8_t L1{4};
            uint8_t L2{64};
            uint16_t L3{1500};
            uint8_t L4_sq{32};
            double delay_res_ns{12.5};

        };

        TxParams txparams;          ///< The Transmission Parameters
        RxParams rxparams;          ///< The Reception Parameters

        double x_max_deg{30.0};     ///< The Max X Direction to Scan
        double x_min_deg{-30.0};    ///< The Min X Direction to Scan
        uint8_t x_steps{60};        ///< The How Many Steps in the X Direction

        double y_max_deg{30.0};     ///< The Max Y Direction to Scan
        double y_min_deg{-30.0};    ///< The Min Y Direction to Scan
        uint8_t y_steps{60};        ///< The Number of Steps in the Y Direction

        double z_min_mm{10.0};      ///< The Minimum Distance To Scan
        double z_max_mm{260.0};     ///< The Maximum Distance to Scan
        double depth_range{250.0};  ///< How Wide we can Scan in the Z Direction
        uint16_t z_steps{2500};     ///< The Number Steps to Scan in the Z Direction

        double focus_rx{0.0};       ///< Focus for Reception in Meters 0: Dynamic
        double focus_tx{0.05};      ///< Focus for Tx in Meters 0 : Plane

        bool usedelays{false};       ///< If we are Using Delays or Coefficients

    };

    /**
     * \brief 
     * 
     */
    struct USParams {
        struct Pulse {

            enum SubType: uint8_t {

                FIFTY_GAIN8 = 0,        ///< 50 ns Pulse Gain of 8
                HUNDRED_GAIN8 = 1,      ///< 100ns Pulse Gain of 8
                FIFTY_GAIN2 = 2,        ///< 50ns Pulse Gain of 2
                FIFTY_GAIN0 = 3,        ///< 50ns Pulse Gain of 0
                HUNDRED_GAIN0 = 4,      ///< 100ns Pulse Gain of 0
                DARK_GAIN8 = 5,         ///< Dark Pulse Gain of 8
                FIFTY_GAIN1 = 6,        ///< 50ns Pulse Gain of 1
                DARK_GAIN0 = 7,         ///< Dark Pulse Gain of 0
                TW_FIVE_GAIN8 = 8,      ///< 25ns Pulse Gain of 8
                FIFTY_NEG_GAIN8 = 9,    ///< 50ns Negative Pulse Gain of 8
                DARK_NEG_GAIN8 = 10,    ///< Dark Negative Pulse Gain of 8
                HUNDRED_NEG_GAIN8 = 11  ///< 100ns Negative Pulse Gain of 8

            };

            /**
            * \brief 
            * 
            */
            enum Type: uint8_t {

                UNIPOLAR = 0    ///< One Peak

            };

            Type type{UNIPOLAR};                   ///< The Shape of the Pulse
            SubType subtype{FIFTY_GAIN8};          ///< The Length and Gain

        };

        Pulse pulse{};              ///< The Pulse Shape and Type
        float attenuation{.5f};     ///< attenuation in dB/MHz*cm , .5 for phantom 1 for water
        double freq_cent_mhz{5.0};  ///< The Center Frequency of the Ultrasound 
        
        TransducerParams trparams;
        ControllerParams conparams;
        ASICParams asicparams;
        FPGAParams fpgaparams;

    };


}