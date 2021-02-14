#pragma once
#include <SDL.h>
#include <thread>

#define BUGL_GREEN 0 , 255 , 0
#define BUGL_BLUE 0, 255, 0
#define BUGL_RED 255 , 0 , 0
#define BUGL_ORANGE 255, 128, 0
#define BUGL_PURPLE 128 , 0 , 255
#define BUGL_BLACK 0 , 0, 0
#define BUGL_GRAY 64 , 64 , 64
#define BUGL_LIGHTGRAY 128 , 128, 128

//misc stuff

void getMax(int X, int Y, int* P);

void getMin(int X, int Y, int* P);

//pixle color class

class PixelColor
{
public:
	char r;
	char g;
	char b;

	//constructor

	PixelColor(int R = 0, int G = 0, int B = 0);

	//print pixel color

	void printColor();

	//get color

	int* getColorArray();

	//set color

	void setColor(int R, int G, int B);
};

//pixel class

class Pixel
{
private:
	short pixelX;
	short pixelY;

	unsigned int pixelDepth;

	PixelColor _pixelColor;

public:
	
	//constructor

	Pixel();

	//intended constructor

	Pixel(int X, int Y);

	PixelColor* getPixelColor();

	void setPixelColor(int R, int G, int B);
	void setPixelColor(PixelColor PC);


};

//canvas class

class Canvas
{
private:
	PixelColor drawColor;
	PixelColor backGround;

	//render chunk

	void renderTop();
	void renderBottom();

	std::thread canvasThread1;
	std::thread canvasThread2;

public:

	Pixel** pixelMatrix; //2 dimensionalarray, hence the pointer pointer

	SDL_Window* canvasWindow;
	SDL_Renderer* canvasRenderer;

	int canvasSizeX;
	int canvasSizeY;

	//defualt constructor

	Canvas();

	//deconstructor

	~Canvas();

	//intended constructor

	Canvas(int dimensionX, int dimensionY);

	//get dimensions

	int* getDimensions();

	//print dimension

	void printDimensions();

	//set brush color

	void setBrushColor(int R, int G, int B);

	//color pixel

	void drawPixel(int X, int Y);

	//draw line

	void drawLine(int X1, int Y1, int X2, int Y2, int resolution = 3);

	//get pixel color

	PixelColor* getPixelColor(int X, int Y);

	//update screen

	void updateScreen(bool clear = true);

	//update screen optimised

	void volatile updateScreenOptimised();

	//set background color

	void setBackground(int R, int G, int B);

};

//renderchunk method

void volatile renderChunk(Canvas* canvas, int startRow, int endRow);
