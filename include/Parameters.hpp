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

#include "ASIC.hpp"

namespace SoundCath {

    class FPGAParams {



    };

    
    class ASICParams {

    public:

        struct CoreSettings {




        };

        /**
         * \brief Enum for the clock speed of the ASIC, Either Low: 25MHz, or High: 100MHz
         */
        enum ClkSpeed : uint8_t {

            LOW = 25,  ///< 25MHz
            HIGH = 100 ///< 100MHz

        };

        /**
         * \brief Set the Clock Speed object
         * 
         * \param speed 
         */
        void SetClockSpeed(const ClkSpeed speed);

        /**
         * \brief 
         * 
         */
        struct BeamTiming {

            double SetupTime;       ///<           
            double RunRxTime;       ///<
            double RunTXTime;       ///<
            double StopTXTime;      ///<
            double StopRXTime;      ///<
            double AnaResetStopTime;///<
            double PreChargeTime;   ///<
        
        };

        /**
         * \brief Set the Beam Timing object
         * 
         * \param timing 
         */
        void SetBeamTiming(const BeamTiming timing);

        /**
         * \brief 
         * 
         */
        struct DRVConfig {

            bool Enable;    ///< If DRV is Enabled
            bool FFen;      ///<
            double Bias;    ///<

        };

        /**
         * \brief 
         * 
         * \param config 
         */
        void SetDRVConfig(const DRVConfig& config);

        /**
         * \brief 
         * 
         */
        struct BModeSettings {

            std::vector<uint8_t> groups{ALLGROUPS};                         ///< The beam forming groups that are active
            std::array<char, 2> axes{'y', 'x'};                             ///< The Steering Axes
            std::array<std::string, 2> steering{"-30:5:30", "-30:3:30"};    ///< The Steering Angle Arrays
            std::array<std::string, 2> mlasteering{"-30:1:30", "-30:1:30"}; ///< The MLA Steering Angle Arrays
            float maxangleradius{30};   ///< The Maximum Angle Magnitude
            bool usequeue{true};        ///< Whether or not to queue up the beams before sending
            float depthrange{.250f};    ///< The Range of the Depth
            float dbrange{-40.0f};      ///< The Power Range

        };

        /**
         * \brief 
         * 
         * \param bmodesetting 
         */
        void SetBModeSetting(const BModeSettings& bmodesetting) { this->bmodesettings = bmodesetting;}

        /**
         * \brief Construct a new Config object
         * \note Fills with the Default Values
         */
        ASICParams() = default;   

        /**
         * \brief Construct a new ASICParams object
         * 
         * \param clkspeed
         * \param bmode
         */
        ASICParams(const ClkSpeed clkspeed, const BModeSettings& bmode);

    private:

        BModeSettings bmodesettings;///< uBeanforming Settings 
        ClkSpeed speed;             ///< ASIC Clock Speed
        DRVConfig drvconfig;        ///< DRV Configuration
        BeamTiming timing;          ///< The Beam Timing

    };

    /**
     * \brief 
     * 
     */
    class USParams {

    public:

        /**
        * \brief 
        * 
        */
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

            /**
             * \brief Construct a new Pulse object
             * 
             * \param type
             * \param subtype
             */
            Pulse(Type type, SubType subtype);
            
            /**
             * \brief Construct a new Pulse object
             * 
             */
            Pulse() = default;

            Type type{UNIPOLAR};                   ///< The Shape of the Pulse
            SubType subtype{FIFTY_GAIN8};          ///< The Length and Gain

        };

        /**
         * \brief Set the Pulse object
         * 
         * \param pulse 
         */
        void SetPulse(const Pulse pulse) { this->pulse = pulse;}

        /**
         * \brief Set the Attenuation object
         * 
         * \param atten
         */
        void SetAttenuation(const float atten) { this->attenuation = atten; }

    private:

        Pulse pulse;        ///< The Pulse Shape and Type
        float attenuation{.5f};  ///< attenuation in dB/MHz*cm , .5 for phantom 1 for water

    };

    class RenderParams {

        float fps{20.0f};   ///< The Rendering Frames Per Second


    };

    struct ControllerParams {


    };

    /**
     * \brief 
     * 
     */
    struct Params {

        RenderParams renderparams; ///< The Parameters for the renderer
        USParams usparams;          ///< The Parameters for the Ultrasound
        ASICParams asicparams;      ///< The Parameters for the ASIC
        FPGAParams fpgaparams;      ///< The Parameters for the FPGA
        ControllerParams controllerparams;  ///< The Parameters For the UltraSound Controller

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
        Params& GetParams() { return this->params; }

    private:

        Params params; ///< The parsed Parameters
    
        /**
         * \brief 
         * 
         */
        void PrintMenu();    

    };
}