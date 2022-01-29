#include "ASIC.hpp"

using namespace SoundCath;


ASIC::ASIC(Interface& face, ClkSpeed speed): driver(face), speed(speed) {}

