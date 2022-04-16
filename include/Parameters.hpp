/**
 * \file main.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
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

namespace SoundCath {

    struct FPGAParams {

        const char* Name{"Neso Artix 7 FPGA Module"};
        bool DisableLED{true};

    };

    struct ASICParams {

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

        enum ClkSpeed : uint8_t {

            LOW = 25,  ///< 25MHz
            HIGH = 100 ///< 100MHz

        };

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

        struct BModeSettings {

            std::array<bool, 64> offgroups{};   ///< The beam forming groups that are inactive
            float maxangle{30.0f};                          ///< The Maximum Angle of the Beams in the Z plane
            uint8_t xres{10};                               ///< The Number of Samples Between 0 and the max angle, total number of x Samples are 2*xres + 1
            uint8_t yres{10};                               ///< The Number of Samples Between 0 and the max angle in the y direction, total samples: 2*yres + 1
            bool usequeue{true};                            ///< Whether or not to queue up the beams before sending
            float depthrange{.250f};                        ///< The Range of the Depth
            float dbrange{40.0f};                           ///< The Power Range

        };

        CoreSettings core;
        BModeSettings bmodesettings;///< uBeanforming Settings 
        ClkSpeed speed{LOW};        ///< ASIC Clock Speed
        DRVConfig drvconfig;        ///< DRV Configuration
        BeamTiming timing;          ///< The Beam Timing

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

        Pulse pulse{};        ///< The Pulse Shape and Type
        float attenuation{.5f};  ///< attenuation in dB/MHz*cm , .5 for phantom 1 for water

    };

    struct GUIParams {


    };

    struct RenderParams {

        float fps{20.0f};   ///< The Rendering Frames Per Second

    };

    struct ControllerParams {

        struct RxParams {

            uint8_t L0{128};       
            uint16_t L1{1024};
            uint16_t L2{8192};
            uint8_t L1MAX{6};
            uint8_t L2MAX{1};
            uint8_t c78factor{16};
            double soundspeed{1490.0};
            double pitch_nm{1.8};
            double group_pitch_nm{7.2};
            double delay_res_ns{20.0};

        };

        struct TxParams {


        };

        TxParams txparams;
        RxParams rxparams;

    };


    /**
     * \brief 
     * 
     */
    class ParameterParser{
    public:

        /**
         * \brief Construct a new Renderer Parameters object
         * 
         * \param argc
         * \param kwargs
         */
        ParameterParser(const int argc, const char* const* const kwargs);

        /**
         * \brief Construct a new Renderer Parameters object
         * 
         */
        ParameterParser() = default;

        /**
         * \brief 
         * 
         * \param argc
         * \param kwargs
         */
        void ParseCL(const int argc, const char* const* const kwargs);

        /**
         * \brief Get the Params object
         * 
         * \return Params& 
         */
        RenderParams& GetParams() { return this->params; }

    private:

        RenderParams params; ///< The parsed Parameters
    
        /**
         * \brief 
         * 
         */
        void PrintMenu();    

    };
}