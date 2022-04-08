/**
 * \file Point.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-25-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <array>
#include <iostream>

namespace SoundCath {

using std::array;
using std::ostream;

class SphericalPoint;
class RectPoint;
class CylindPoint;

/**
 * \brief 
 * 
 */

class SphericalPoint {

public:

    /**
     * \brief Construct a new Spherical Point object
     * 
     */
    SphericalPoint() = default;

    /**
     * \brief Construct a new Spherical Point object
     * 
     * \param r
     * \param phi
     * \param theta
     */
    SphericalPoint(double r = 0.0f, double phi = 0.0f, double theta = 0.0f);
    
    /**
     * \brief Construct a new Spherical Point object
     * 
     * \param point
     */
    SphericalPoint(const RectPoint& point);

    /**
     * \brief Destroy the Spherical Point object
     * 
     */
    ~SphericalPoint();

    /**
     * \brief 
     * 
     * \return double 
     */
    double GetR();

    /**
     * \brief Get the Phi object
     * 
     * \return double 
     */
    double GetPhi();

    /**
     * \brief Get the Theta object
     * 
     * \return double 
     */
    double GetTheta();

    /**
     * \brief 
     * 
     * \param r
     */
    void SetR(const double r);

    /**
     * \brief Set the Phi object
     * 
     * \param phi
     */
    void SetPhi(const double phi);

    /**
     * \brief Set the Theta object
     * 
     * \param theta
     */
    void SetTheta(const double theta);

    /**
     * \brief 
     * 
     * \param os
     * \param point
     * \return ostream& 
     */
    friend ostream& operator<<(ostream& os, const SphericalPoint& point);
    
    /**
     * \brief 
     * 
     * \param point
     */
    void operator=(const SphericalPoint& point);
    
    /**
     * \brief 
     * 
     * \param point
     * \return true 
     * \return false 
     */
    bool operator==(const SphericalPoint& point);

private:

    double r;    ///< The Distance from the Origin
    double phi;  ///< The Angle from the Y Axis
    double theta;///< The Angle on the XY Axis: Same as Polar Theta

};

/**
 * \brief 
 * 
 */
class RectPoint {

public:

    /**
     * \brief Construct a new Rect Point object
     * 
     */
    RectPoint() = default;

    /**
     * \brief Construct a new Rect Point object
     * 
     * \param point
     */
    RectPoint(SphericalPoint& point);

    /**
     * \brief Construct a new Rect Point object
     * 
     * \param point
     */
    RectPoint(CylindPoint& point);

    /**
     * \brief Construct a new Rect Point object
     * 
     * \param x
     * \param y
     * \param z
     */
    RectPoint(double x = 0.0f, double y = 0.0f, double z = 0.0f);

    /**
     * \brief 
     * 
     * \return double 
     */
    double GetX() const noexcept { return this->x; };

    /**
     * \brief 
     * 
     * \return double 
     */
    double GetY() const noexcept { return this->y; } 

    /**
     * \brief 
     * 
     * \return double 
     */
    double GetZ() const noexcept { return this->z; }

    /**
     * \brief 
     * 
     * \param x
     */
    void SetX(const double x);

    /**
     * \brief 
     * 
     * \param y
     */
    void SetY(const double y);

    /**
     * \brief 
     * 
     * \param z
     */
    void SetZ(double z);

    // Operator Overloading 

    /**
     * \brief 
     * 
     * \param os
     * \param point
     * \return ostream& 
     */
    friend ostream& operator<<(ostream& os, RectPoint& point);
    
    /**
     * \brief 
     * 
     * \param point
     */
    void operator=(RectPoint& point);
    
    /**
     * \brief 
     * 
     * \param point
     * \return true 
     * \return false 
     */
    bool operator==(RectPoint& point);

private:

    double x;   ///< The X Component
    double y;   ///< The Y Component
    double z;   ///< The Z Component

};

/**
 * \brief 
 * 
 */
class CylindPoint {

public:

    // ----------------------- Default Constructor --------------------- //

    /**
     * \brief Construct a new Cylind Point object
     * 
     */
    CylindPoint() = default;

    // ----------------------- Copy Constructor ------------------------- //

    /**
     * \brief Construct a new Cylind Point object
     * 
     * \param point
     */
    CylindPoint(const CylindPoint& point);

    //  ----------------- Conversion Constructors -------------------------- //

    /**
     * \brief Construct a new Cylind Point object
     * 
     * \param r
     * \param theta
     * \param z
     */
    CylindPoint(double r = 0.0f, double theta = 0.0f, double z = 0.0f);

    /**
     * \brief Construct a new Cylind Point object
     * 
     * \param point
     */
    CylindPoint(const RectPoint& point);
    
    /**
     * \brief Construct a new Cylind Point object
     * 
     * \param point
     */
    CylindPoint(const SphericalPoint& point);

    // -------------------- Destructor ---------------------------------- //

    /**
     * \brief Destroy the Cylind Point object
     * 
     */
    ~CylindPoint();

    // ----------------------------- Getters -------------------------- //
    /**
     * \brief 
     * 
     * \return double 
     */
    double GetR();

    /**
     * \brief 
     * 
     * \return double 
     */
    double GetZ();

    /**
     * \brief Get the Theta object
     * 
     * \return double 
     */
    double GetTheta();

    // ------------------------------ Setters --------------------------- //

    /**
     * \brief 
     * 
     * \param r
     */
    void SetR(const double r);

    /**
     * \brief Set the Theta object
     * 
     * \param theta
     */
    void SetTheta(const double theta);

    /**
     * \brief 
     * 
     * \param z
     */
    void SetZ(const double z);

    // ----------------------- Operators -------------------------------- //

    /**
     * \brief 
     * 
     * \param os
     * \param point
     * \return ostream& 
     */
    friend ostream& operator<<(ostream& os, CylindPoint& point);
    
    /**
     * \brief 
     * 
     * \param point
     */
    void operator=(CylindPoint& point);
    
    /**
     * \brief 
     * 
     * \param point
     * \return true 
     * \return false 
     */
    bool operator==(CylindPoint& point);

private: 

    double r;       ///< The Distance From the Origin
    double theta;   ///< The Angle On the XY Plane
    double z;       ///< The Height ( Z Component )

};

}