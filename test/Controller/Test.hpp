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

namespace SoundCath {

class ControllerTester {




private:

    RXControllerTester rxtester;
    TXControllerTester txtester;

};

class RXControllerTester {

public:

    bool TestTaylorCompression();

    bool TestTaylorDecompression();

    bool TestDynTaylorCompression();

    bool TestDynTaylorDecompression();

private:

    RXController rx;

};

class TXControllerTester {

public:

    bool TestTaylorCompression();

    bool TestTaylorDecompression();

    bool TestDynTaylorCompression();

    bool TestDynTaylorDecompression();

    bool TestGenerateDelays();

    bool Test



};

}