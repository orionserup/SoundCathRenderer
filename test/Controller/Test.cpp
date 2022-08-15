/**
 * \file Test.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 02-08-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Test.hpp"

#include <chrono>

using SoundCath::ControllerTester;
using SoundCath::RXControllerTester;

template<ControllerParams::RxParams params, TransducerParams tparams>
void RXControllerTester<params, tparams>::Benchmark(const uint32_t size) {

    static auto start = std::chrono::high_precision_clock




};

