#include<Windows.h>
#include<WinUser.h> //winuser and windows for sounds and input
#include<stdio.h> //stdio for debugging
#include<SDL.h> //SDL is a BUGL dependancy

//now i'm going to include my self written libraries!!
//BUGL stands for "Bailey's Utility and Graphics Library"

#include "BUGL_Events.h"
#include "BUGL_Graphics.h"

//other includes

#include "Stages.h"

//main

int main(int argc, char* argv[])
{
	//set up canvas and event handler

	Canvas* c = new Canvas(750, 750);
	EventHandler* e = new EventHandler();

	//set up various variables

	int canvasPosX;
	int canvasPosY;

	//set up canvas parameters
	
	c->setBrushColor(BUGL_BLUE);
	c->setBackground(BUGL_GRAY);

	float i = 0;

	PLAYINTRO(c->canvasRenderer, c);
	SCENE_TWO(c);
	SCENE_THREE(c);
	SCENE_FOUR(c);
	SCENE_FIVE(c);

	//drawHuman(c, 250, 250, 20);

	//c->updateScreen();

	//Sleep(1000);

	return 0;
}