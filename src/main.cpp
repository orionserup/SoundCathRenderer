/**
 * \file main.cpp
 * \author Orion Serup (orionserup@gmail.com)
 * \brief Contains the Main Function
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
 * \brief The Program Entry Point
 * 
 * \param argc: The Number of Arguments
 * \param kwargs: The Actual Arguments
 * \return int: The Status of the Program on Exit
 */
int main(int argc, const char** kwargs) {

	Controller cont; 

	Renderer ren;
	GUI gui(ren);

	gui.SetTitle("Real Time Render");
	gui.SetSize(600, 400);
	gui.SetFPS(30);


	return EXIT_SUCCESS;
}
