/**
 * \file GUI.hpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 2022-01-28
 * 
 * \copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "Parameters.hpp"

#include <vtkRenderer.h>
#include <vtkNew.h>

#include "Renderer.hpp"

#include <cstdint>

namespace SoundCath {

/**
 * \brief GUI Implementation using VTK
 * 
 * \todo Everything
 * 
 */
class GUI {

public:


    /**
     * \brief Set the Height object
     * 
     * \param height
     */
    void SetHeight(const uint16_t height);

    /**
     * \brief Set the Width object
     * 
     * \param width
     */
    void SetWidth(const uint16_t width);

    /**
     * \brief Set the Dimen object
     * 
     * \param width
     * \param height
     */
    void SetSize(const uint16_t width, uint16_t height);

    /**
     * \brief Set the Renderer object
     * 
     * \param renderer
     */
    void SetRenderer(const Renderer& renderer);

    /**
     * \brief 
     * 
     */
    void Draw();

private:

    vtkRenderWindow* window;
    //Renderer& renderer;
    GUIParams params;

};

}