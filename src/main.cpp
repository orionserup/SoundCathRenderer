/**
 * \file main.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief 
 * \version 0.1
 * \date 01-28-2022
 * 
 * \copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include "GUI.hpp"
#include "Controller.hpp"

using namespace SoundCath;

/**
 * \brief 
 * 
 * \param argc
 * \param kwargs
 * \return int 
 */
int main(int argc, char** kwargs) {

	Controller cont; 

	Renderer ren;
	GUI gui(ren);

	gui.SetTitle("Real Time Render");
	gui.SetSize(600, 400);
	gui.SetFPS(30);


	return EXIT_FAILURE;
}
