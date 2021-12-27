/**
* @file ASIC.hpp
* @authors Orion Serup (orion@soundcath.com)
*/

#pragma once 

#include "main.hpp"
#include "Interface.hpp"
#include <string>
#include <exception>

namespace SoundCath {

    class ASIC {

    public:

        ASIC() = default;
        ~ASIC();

        void SendBeam();

    private:

        ASICInterface asic;

    };

    struct ASICException {
        ASICException(const std::string& message) { this->message = message; }
        std::string message;
    };

}
