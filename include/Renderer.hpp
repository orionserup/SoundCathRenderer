/**
 * \file Renderer.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief Contains the declarations and types for the rendering component
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <vector>
#include <array>
#include <iostream>

#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>

namespace SoundCath {

/**
 * \brief Renders the Data to a File or GUI or Both
 * 
 * \todo Implement everything, sorry
 * 
 */
class Renderer {

public:

    /**
     * \brief  Renderer Construtor for an empty Canvas
     * \note   Default
     * \retval Renderer Instance
     */
    Renderer() = default;

    /**
     * \brief  Renderer Constructor for a non empty canvas 
     * \note   Used with initial run
     * \param  points: all of the points in the canvas so far
     * \retval Renderer Instance
     */
    Renderer(const vtkPoints* const points);

    /**
     * \brief Construct a new Renderer object
     * 
     * \param filename: File to Save the Video to
     */
    Renderer(const std::string filename);

    /**
     * \brief 
     * 
     */
    void SmoothSurface();

    /**
     * \brief  Draws the Points to the screen and renders a model
     * \note   Is a 3D Model
     * \retval None
     */
    void Render();

    /**
     * \brief 
     * 
     * \param filename
     */
    void SaveToFile(const std::string filename);


private:

    // vtkNew<vtkPoints> points[2];        ///< Two Sets of Points As a Buffer
    // vtkNew<vtkPolyData> volume;         ///< Generate a Volume from the Points
    // vtkNew<vtkPolyDataMapper> mapper;   ///< Point Mapper to Volume 

    // vtkNew<vtkRenderer> renderer;       ///< Turns the 3D Polygonal data to a viewable object

    std::string filename;                    ///< What File to Save it to If Any

};

}