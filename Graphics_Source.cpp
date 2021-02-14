#include"BUGL_Graphics.h"
#include<SDL.h>
#include<stdio.h>
#include<numeric>
#include<algorithm>
#include <Windows.h>
#include <thread>

#define BUGL_GREEN 0 , 255 , 0
#define BUGL_BLUE 0, 255, 0
#define BUGL_RED 255 , 0 , 0
#define BUGL_ORANGE 255, 128, 0
#define BUGL_PURPLE 128 , 0 , 255
#define BUGL_BLACK 0 , 0, 0
#define BUGL_GRAY 64 , 64 , 64
#define BUGL_LIGHTGRAY 128 , 128, 128

//max
void getMax(int X, int Y, int* P)
{
	if(X > Y)
	{
		*P = X;
	}
	else
	{
		*P = Y;
	}
}

//min

void getMin(int X, int Y, int* P)
{
	if (X < Y)
	{
		*P = X;
	}
	else
	{
		*P = Y;
	}
}

//renderchunk

void volatile renderChunk(Canvas* Canvas, int start, int finish)
{
	SDL_RenderClear(Canvas->canvasRenderer);

	//fill pixels according to pixel matrix

	for (int i = start; i < finish; i++)
	{
		for (int j = 0; j < Canvas->canvasSizeY; j++)
		{
			PixelColor* tempColor;
			Pixel* tempPixle;

			tempPixle = &(Canvas->pixelMatrix[i][j]);
			tempColor = tempPixle->getPixelColor();

			SDL_SetRenderDrawColor(Canvas->canvasRenderer, tempColor->r, tempColor->g, tempColor->b, 255);

			SDL_RenderDrawPoint(Canvas->canvasRenderer, i, j);

		}
	}


	return;
}

//default constructor

Canvas::Canvas()
{
	canvasSizeX = NULL;
	canvasSizeY = NULL;

	PixelColor* temp = new PixelColor();
	drawColor = *(temp);

	delete temp;

	//set everything to null

	canvasWindow = NULL;
	canvasRenderer = NULL;

	pixelMatrix = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);
}

//intended constructor

Canvas::Canvas(int dimensionX, int dimensionY)
{

	canvasSizeX = dimensionX;
	canvasSizeY = dimensionY;

	drawColor = *(new PixelColor());

	pixelMatrix = new Pixel* [dimensionX];

	//create pixel matrix

	for(int i = 0; i < dimensionX; i++)
	{
		pixelMatrix[i] = new Pixel[dimensionY];
	}

	//empty pixel matrix

	for(int i = 0; i < dimensionX; i++)
	{
		for(int j = 0; j < dimensionY; j++)
		{
			Pixel* tempP = new Pixel();
			pixelMatrix[i][j] = *(tempP);

			delete tempP;
		}
	}

	SDL_Init(SDL_INIT_EVERYTHING);

	//create window

	SDL_CreateWindowAndRenderer(dimensionX, dimensionY, NULL, &canvasWindow, &canvasRenderer);

	updateScreen();
}

//get dimensions

int* Canvas::getDimensions()
{
	int X = canvasSizeX;
	int Y = canvasSizeY;

	int returnArr[2];

	returnArr[0] = X;
	returnArr[1] = Y;

	return returnArr;


}

//print dimensions

void Canvas::printDimensions()
{
	printf("X:%d\nY:%d\n", canvasSizeX, canvasSizeY);
}

//set draw color

void Canvas::setBrushColor(int R, int G, int B)
{
	drawColor.setColor(R, G, B);

	SDL_SetRenderDrawColor(canvasRenderer, R, G, B, 255);
}

//render top

void Canvas::renderTop()
{
	Pixel* tempPixle;
	
	//fill pixels according to pixel matrix

	for (int i = 0; i < canvasSizeX / 2; i++)
	{
		for (int j = 0; j < canvasSizeY; j++)
		{
			PixelColor* tempColor;
			
			tempPixle = &pixelMatrix[i][j];
			tempColor = tempPixle->getPixelColor();

			SDL_SetRenderDrawColor(canvasRenderer, tempColor->r, tempColor->g, tempColor->b, 255);

			SDL_RenderDrawPoint(canvasRenderer, i, j);
		}
	}

	return;
}

//render bottom

void Canvas::renderBottom()
{
	Pixel* tempPixle;
	
	//fill pixels according to pixel matrix

	for (int i = canvasSizeX / 2; i < canvasSizeX; i++)
	{
		for (int j = 0; j < canvasSizeY; j++)
		{
			PixelColor* tempColor;
			
			tempPixle = &pixelMatrix[i][j];
			tempColor = tempPixle->getPixelColor();

			SDL_SetRenderDrawColor(canvasRenderer, tempColor->r, tempColor->g, tempColor->b, 255);

			SDL_RenderDrawPoint(canvasRenderer, i, j);
		}
	}

	return;
}

//update screen optimised

void volatile Canvas::updateScreenOptimised()
{
	//clear buffer

	SDL_RenderClear(canvasRenderer);

	//assign threads

	std::thread th1(&renderChunk, this, 0, 165);
	std::thread th2(&renderChunk, this, 166, 332);
	std::thread th3(&renderChunk, this, 333, 500);

	//destroy threads

	th1.join();
	th1.~thread();
	th2.join();
	th2.~thread();
	th3.join();
	th3.~thread();

	//show render

	SDL_RenderPresent(canvasRenderer);

	//printf("\t\trender presented\n");

	//clear screen

	Pixel* pointPixle;

	for (int i = 0; i < canvasSizeX; i++)
	{
		for (int j = 0; j < canvasSizeY; j++)
		{
			pointPixle = &pixelMatrix[i][j];
			pointPixle->setPixelColor(backGround.r, backGround.g, backGround.b);
		}
	}

	SDL_Event e;

	SDL_PollEvent(&e);

	if (e.type == SDL_QUIT)
	{
		SDL_DestroyRenderer(canvasRenderer);
		SDL_DestroyWindow(canvasWindow);

		printf("SDL QUIT!");
		exit(0);
	}

}

//update screen

void Canvas::updateScreen(bool clear)
{
	SDL_RenderClear(canvasRenderer);

	Pixel* pointPixle;
	Pixel* tempPixle;
	PixelColor* tempColor;

	//fill pixels according to pixel matrix

	for(int i = 0; i < canvasSizeX; i++)
	{
		for(int j = 0; j < canvasSizeY; j++)
		{
			tempPixle = &pixelMatrix[i][j];
			tempColor = tempPixle->getPixelColor();

			SDL_SetRenderDrawColor(canvasRenderer, tempColor->r, tempColor->g, tempColor->b, 255);

			SDL_RenderDrawPoint(canvasRenderer, i, j);
		}
	}

	SDL_RenderPresent(canvasRenderer);

	//clear screen

	if(clear)
	{
		for (int i = 0; i < canvasSizeX; i++)
		{
			for (int j = 0; j < canvasSizeY; j++)
			{
				pointPixle = &pixelMatrix[i][j];
				pointPixle->setPixelColor(backGround.r, backGround.g, backGround.b);
			}
		}
	}

	SDL_Event e;

	SDL_PollEvent(&e);

	if(e.type == SDL_QUIT && e.type == SDL_WINDOWEVENT)
	{
		const char* s = SDL_GetError();

		SDL_DestroyRenderer(canvasRenderer);
		SDL_DestroyWindow(canvasWindow);

		printf("ERORR: %s\n", s);
		printf("SDL QUIT!");
		exit(0);
	}
}

//color pixel

void Canvas::drawPixel(int X, int Y)
{
	if (X < canvasSizeX && Y < canvasSizeY && X > 0 && Y > 0)
	{
		Pixel* toColor = &pixelMatrix[X][Y];

		toColor->setPixelColor(drawColor);
	}
}

//draw line

void Canvas::drawLine(int _X1, int _Y1, int _X2, int _Y2, int r)
{
	//set all vars

	int X1 = _X1;
	int Y1 = _Y1;

	int X2 = _X2;
	int Y2 = _Y2;

	//get the delta (could be negative)

	float deltaX = X2 - X1;
	float deltaY = Y2 - Y1;

	//get real distance

	float pointDistance = sqrt(pow(abs(deltaX), 2) + pow(abs(deltaY), 2));

	//get step size (could be negative)

	float stepX = (deltaX / pointDistance);
	float stepY = (deltaY / pointDistance);

	//printf("stepX : %f\t stepY : %f\n", stepX, stepY);

	//draw size

	float drawX;
	float drawY;

	drawX = X1;
	drawY = Y1;

	for (int i = 0; i < pointDistance; i++)
	{
		drawX += stepX;
		drawY += stepY;

		drawPixel(drawX, drawY);
	}
}

//set background color

void Canvas::setBackground(int R, int G, int B)
{
	backGround.setColor(R, G, B);
}

//get pixel color

PixelColor* Canvas::getPixelColor(int X, int Y)
{
	if(X > canvasSizeX || Y > canvasSizeY)
	{
		return NULL;
	}
	
	Pixel* px = &pixelMatrix[X][Y];

	return px->getPixelColor();
}

//pixel color

PixelColor::PixelColor(int R, int G, int B)
{
	r = R;
	g = G;
	b = B;
}

//print colors

void PixelColor::printColor()
{
	printf("PIXEL COLOR:\nR: %d\nG: %d\nB: %d\n", r, g, b);
}

//get color array

int* PixelColor::getColorArray()
{
	int toReturn[3];

	toReturn[0] = r;
	toReturn[1] = g;
	toReturn[2] = b;

	return toReturn;
}

//set pixel color

void PixelColor::setColor(int R, int G, int B)
{
	r = R;
	g = G;
	b = B;
}

//pixle default construtor

Pixel::Pixel()
{
	pixelX = -1;
	pixelY = -1;
	pixelDepth = -1;

	_pixelColor = *(new PixelColor());
}

//intended pixel constructor

Pixel::Pixel(int X, int Y)
{
	pixelDepth = 0;
	pixelX = X;
	pixelY = Y;

	_pixelColor = *(new PixelColor());
}

//get pixel color

PixelColor* Pixel::getPixelColor()
{
	PixelColor* p = &_pixelColor;
	return p;
}

//set color

void Pixel::setPixelColor(int R, int G, int B)
{
	_pixelColor.setColor(R, G, B);
}

void Pixel::setPixelColor(PixelColor PC)
{
	_pixelColor = PC;
}

//canvas deconstructor

Canvas::~Canvas()
{
	delete pixelMatrix;
	delete canvasRenderer;
	delete canvasWindow;
}
