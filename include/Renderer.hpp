/**
 * @brief  
 * @note   
 * @retval None
 */

#pragma once

#include <vector>
#include <array>


namespace SoundCath{

<<<<<<< HEAD
=======
/**
 * @brief  
 * @note   
 * @retval None
 */
class Point {

public:

    /**
     * @brief  
     * @note   
     * @retval 
     */
    Point() = default;

    /**
     * @brief  
     * @note   
     * @param  R: 
     * @param  xdeg: 
     * @param  ydeg: 
     * @retval 
     */
    Point(const float R, const float xdeg, const float ydeg);

    /**
     * @brief  
     * @note   
     * @param  R: 
     * @retval None
     */
    void SetR(const float R);

    /**
     * @brief  
     * @note   
     * @param  XDeg: 
     * @retval None
     */
    void SetXDeg(const float XDeg);

    /**
     * @brief  
     * @note   
     * @param  YDeg: 
     * @retval None
     */
    void SetYDeg(const float YDeg);

    float GetR();
    float GetXDeg();
    float GetYDeg();

private:

    std::array<float, 3> point; //< Storage of the 3D coordinates

};

>>>>>>> parent of f887b8f (added docs)
class Renderer {

public:

    /**
     * @brief  Renderer Construtor for an empty Canvas
     * @note   Default
     * @retval Renderer Instance
     */
    Renderer() = default;

    /**
     * @brief  Renderer Constructor for a non empty canvas 
     * @note   Used with initial run
     * @param  points: all of the points in the canvas so far
     * @retval Renderer Instance
     */
    Renderer(const std::vector<Point&>& points);
    
    /**
     * @brief  Draws the Points to the screen and renders a model
     * @note   Is a 3D Model
     * @retval None
     */
    void Render();
    
    /**
     * @brief  
     * @note   
     * @param  point: 
     * @retval None
     */
    void AddPoint(const Point& point);

    /**
     * @brief  
     * @note   
     * @retval None
     */
    void ClearPoints();

private:

    std::vector<Point> points;  //< Vector of points to store and display
    uint32_t index;             //< Current index of the vector


};
}