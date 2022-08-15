/**
 * \file Test.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 02-08-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Controller.hpp"
#include "Parameters.hpp"

namespace SoundCath {

/**
 * \brief Tests the Controller Class For Its Functionality, RX and TX plus Queueing
 * 
 * \tparam params: What Controller Params to test the Controller With
 * \tparam tparams: What Transducer to test the Controller With
 */
template<ControllerParams params, TransducerParams tparams>
class ControllerTester {




private:

    RXControllerTester<params, tparams> rxtester;
    TXControllerTester<params, tparams> txtester;

};

/**
 * \brief Tests the Functionality of the RX Controller Class
 * 
 * \tparam params: What RX Params To Test With
 * \tparam tparams: What Transducer Params to test with
 */
template<ControllerParams::RxParams params, TransducerParams tparams>
class RXControllerTester {

public:

    /**
     * \brief Runs Tests on the \ref RXController::CompressTaylor Function
     * \test Tests that Given A Focus Point and Parameters the Controller will Generate the Correct Taylor Coefficients
     * \return true: If the Tests pass
     * \return false: If one or more tests fail
     */
    bool TestTaylorCompression() noexcept;

    /**
     * \brief 
     * \test 
     * \return true 
     * \return false 
     */
    bool TestTaylorDecompression() noexcept;

    /**
     * \brief 
     * 
     * \return true 
     * \return false 
     */
    bool TestDynTaylorCompression() noexcept;

    /**
     * \brief 
     * \test 
     * \return true 
     * \return false 
     */
    bool TestDynTaylorDecompression() noexcept;

    /**
     * \brief 
     * 
     * \param size
     */
    void Benchmark(const uint32_t size);

    /**
     * \brief 
     * 
     * \return auto& 
     */
    auto& GetRXController() noexcept { return rx; }

private:

    RXController<params, tparams> rx; ///< The RX Controller Under Test

};

template<ControllerParams::RxParams params, TransducerParams tparams>
class TXControllerTester {

public:

    /**
     * \brief 
     * 
     * \return true 
     * \return false 
     */
    bool TestTaylorCompression();

    /**
     * \brief 
     * 
     * \return true 
     * \return false 
     */
    bool TestTaylorDecompression();

    /**
     * \brief 
     * 
     * \return true 
     * \return false 
     */
    bool TestDynTaylorCompression();

    /**
     * \brief 
     * 
     * \return true 
     * \return false 
     */
    bool TestDynTaylorDecompression();

    /**
     * \brief 
     * 
     * \return true 
     * \return false 
     */
    bool TestGenerateDelays();

    /**
     * \brief 
     * 
     * \param size
     */
    void Benchmark(const uint32_t size);

private:

    TXController<params, tparams> tx; ///< The Device Under Test

};

}