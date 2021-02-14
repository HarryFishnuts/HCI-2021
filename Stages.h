#pragma once
#include <SDL.h>
#include <BUGL_Graphics.h>

void PLAYINTRO(SDL_Renderer* r, Canvas* c);

void SCENE_TWO(Canvas* c);

void SCENE_THREE(Canvas* c);

void SCENE_FOUR(Canvas* c);

void SCENE_FIVE(Canvas* c);

int attachConsoleToWindow(SDL_Window* w);

int drawLineS(Canvas* c, int X1, int Y1, int X2, int Y2, int offsetX, int offsetY, float scale);

int drawVirus(Canvas* c, int X, int Y, float S);

void drawHuman(Canvas* c, int X, int Y, float S);

void drawFace(Canvas* c, int X, int Y, float S);

void drawFaceReversed(Canvas* c, int X, int Y, float S);

void drawExcalimation(Canvas* c, int X, int Y, float S);

void drawVaccine(Canvas* c, int X, int Y, float S);

//just for organisation, i made Human a class

class Human
{
public:

	Human(); //constructor

	bool infected = false;
	bool colliding = false; //ARRRRRRRG I HATE THIS, THIS IS FOR DEBUG OOONLY!!!!!!!!!!

	int R = 0;
	int G = 255;
	int B = 0;

	int HS = 0; //0 for healthy, 1 for infected, 2 for mask, 3 for vaccined, 4 for doctor

	int X;
	int Y;

	int MX;
	int MY;
};
