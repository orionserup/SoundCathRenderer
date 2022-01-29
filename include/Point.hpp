/**
 * \file Point.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-25-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include <array>
#include <iostream>

namespace SoundCath {

using std::array;
using std::ostream;

class SphericalPoint {

public:

    SphericalPoint() = default;
    SphericalPoint(double r, double phi, double theta);
    SphericalPoint(const RectPoint& point);

    ~SphericalPoint();

    double GetR();
    double GetPhi();
    double GetTheta();

    void SetR(const double r);
    void SetPhi(const double phi);
    void SetTheta(const double theta);

    friend ostream& operator<<(ostream& os, const SphericalPoint& point);
    void operator=(const SphericalPoint& point);
    bool operator==(const SphericalPoint& point);

private:

    double r, phi, theta;

};

class RectPoint {

public:

    RectPoint() = default;
    RectPoint(SphericalPoint& point);
    RectPoint(CylindPoint& point);
    RectPoint(double x = 0.0f, double y = 0.0f, double z = 0.0f);

    ~RectPoint();

    double GetX();
    double GetY();
    double GetZ();

    void SetX(double x);
    void SetY(double y);
    void SetZ(double z);

    // Operator Overloading 

    friend ostream& operator<<(ostream& os, RectPoint& point);
    void operator=(RectPoint& point);
    bool operator==(RectPoint& point);


private:

    double x, y, z;


};

class CylindPoint {

public:

    CylindPoint() = default;
    CylindPoint(const CylindPoint& point);

    CylindPoint(double r = 0.0f, double theta = 0.0f, double z = 0.0f);
    CylindPoint(const RectPoint& point);
    CylindPoint(const SphericalPoint& point);

    ~CylindPoint();

    double GetR();
    double GetZ();
    double GetTheta();

    void SetR(const double r);
    void SetTheta(const double theta);
    void SetZ(const double z);

    friend ostream& operator<<(ostream& os, CylindPoint& point);
    void operator=(CylindPoint& point);
    bool operator==(CylindPoint& point);

private: 

    double r, theta, z;

};



}