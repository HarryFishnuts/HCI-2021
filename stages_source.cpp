#include "Stages.h"

#include <SDL.h>
#include <Windows.h>
#include <BUGL_Graphics.h>
#include <iostream>
#include <thread>
#include <random>



//intro (unorthodox naming scheme, but it helps distinguish the important functions)

void PLAYINTRO(SDL_Renderer* r, Canvas* c)
{
	//clear screen incase not cleared already
	system("cls");

	int i = 0; //iterator

	while (1) //would have used a for loop, but too lazy to get the timing right
	{
		attachConsoleToWindow(c->canvasWindow);

		drawVirus(c, c->canvasSizeX / 2, c->canvasSizeY / 2, i);

		c->updateScreen();

		i++;

		if(i > 30)
		{
			break;
		}
		
	}

	//sleep for .5 seconds

	Sleep(500);

	//print and beep

	printf("CORONAVIRUS\n");
	Beep(400, 500);

	Sleep(200); //beep seems to lag behind main thread so i always put a delay

	c->setBackground(BUGL_BLACK); //set color to green
	c->updateScreen();

	for(int i = 0; i < c->canvasSizeX + 50; i += 50) //loop thru entire screen
	{
		if(i % 100 == 0)
		{
			c->setBrushColor(0 , 0, 255); //set color to blue
		}
		else
		{
			c->setBrushColor(BUGL_RED);
		}
		
		for(int j = 0; j < c->canvasSizeY + 50; j += 50)
		{
			drawVirus(c , i, j, 5); //layering them for thickness
		}

		c->updateScreen(false); //update without cls

		Beep(400, 100);
		Sleep(50); //sleep
	}

	Sleep(100); //brief pause

}

//scene 2

void SCENE_TWO(Canvas* c)
{
	//people count
	const int PCOUNT = 25;

	//set background color
	c->setBackground(0, 0, 0);

	//clear screen
	system("cls");

	//give description

	printf("SIMULATING THE SPREAD\n");

	Beep(450, 500); //beep

	Sleep(1000);

	Human* hArr = new Human[PCOUNT]; //array of 20 humans

	//infection count

	int infectionCount = 1;

	//time iteration, for movement refining

	long moveTime = 0;

	printf("HEALTHY: %d\nINFECTED: %d\n", PCOUNT - infectionCount, infectionCount);

	//simulate humans

	hArr[0].infected = true;
	hArr[0].G = 0;
	hArr[0].R = 255;

	hArr[0].MX = (rand() % 10) - 5;
	hArr[0].MY = (rand() % 10) - 5;

	for(int i = 1; i < PCOUNT; i++) //place each human at random position
	{
		hArr[i].X = (rand() % c->canvasSizeX);
		hArr[i].Y = (rand() % c->canvasSizeY);

		hArr[i].MX = (rand() % 10) - 5;
		hArr[i].MY = (rand() % 10) - 5;
	}

	while (1) //forever loop
	{
		

		moveTime++;

		//update background so it gets redder every infection

		c->setBackground(infectionCount,0,0);
		
		for(int i = 0; i < PCOUNT; i++)
		{
			c->setBrushColor(hArr[i].R, hArr[i].G, hArr[i].B); //set color

			drawHuman(c, hArr[i].X, hArr[i].Y, 5); //draw human

			if(hArr[i].X > c->canvasSizeX) //check if human is out of bounds
			{
				hArr[i].X = 0;
			}

			if (hArr[i].X < 0) //check if human is out of bounds
			{
				hArr[i].X = c->canvasSizeX;
			}

			if (hArr[i].Y > c->canvasSizeY) //check if human is out of bounds
			{
				hArr[i].Y = 0;
			}

			if (hArr[i].Y < 0) //check if human is out of bounds
			{
				hArr[i].Y = c->canvasSizeY;
			}

			//update human position... do it faster if infected

			int r = (rand() % 5) + 5;

			if(moveTime % r == 0)
			{
				hArr[i].MX = (rand() % 16) - 8;
				hArr[i].MY = (rand() % 16) - 8;
			}

			hArr[i].X += hArr[i].MX;
			hArr[i].Y += hArr[i].MY;

			//some thing

			Human checkHuman = hArr[i];

			//check for collisions

			for(int j = 0; j < PCOUNT; j++)
			{
				if(checkHuman.X > hArr[j].X - 30 && checkHuman.X < hArr[j].X + 30) //check for X collision
				{
					if (checkHuman.Y > hArr[j].Y - 50 && checkHuman.Y < hArr[j].Y + 30) //check for Y collision
					{	

						//check for infected

						if(hArr[j].infected)
						{
							if (hArr[i].infected == false)
							{
								infectionCount++;

								system("cls"); //clear

								printf("HEALTHY: %d\nINFECTED: %d\n", PCOUNT - infectionCount, infectionCount);
							}


							hArr[i].infected = true;
							hArr[i].G = 0;
							hArr[i].R = 255;
							
						}


					}
				}
			}

		}

		//check if everyone is infected

		if (infectionCount >= PCOUNT)
		{
			break;
		}

		if(GetKeyState(VK_RETURN) < -5)
		{
			break;
		}

		c->updateScreen();

	}
}

//STAGE 3

void SCENE_THREE(Canvas* c)
{
	attachConsoleToWindow(c->canvasWindow);

	//cover screen in virus sprite
	
	for(int i = 0; i < c->canvasSizeX + 20; i += 100)
	{
		for(int j = 0; j < c->canvasSizeY + 20; j += 100)
		{
			c->setBrushColor(255, 128, 0);
			drawVirus(c, i, j, 10);
		}

		Sleep(100);
		Beep(300, 100);
		c->updateScreen(false);
	}

	Sleep(1000);

	c->setBackground(BUGL_BLACK);
	c->updateScreen();

	//draw new face (green)

	printf("That is a human\n");

	c->setBrushColor(BUGL_GREEN);
	drawFace(c, 200, c->canvasSizeY / 2, 20);
	c->updateScreen();

	Beep(500, 200);
	Sleep(100);

	printf("When infected by a virus...\n");

	//make virus slowly go towards face
	
	for(int i = c->canvasSizeX; i > (c->canvasSizeX / 2) - 50; i -= 30)
	{
		//draw face

		c->setBrushColor(BUGL_GREEN);
		drawFace(c, 200, c->canvasSizeY / 2, 20);

		//draw virus and move it closer

		c->setBrushColor(255, 0, 0);
		drawVirus(c, i, c->canvasSizeY / 2, 10);

		c->updateScreen();

		Beep(250, 100);
		Sleep(50);
	}

	Sleep(500);

	//draw face but red and make a beep sound

	c->setBrushColor(255, 0, 0);
	drawFace(c, 200, c->canvasSizeY / 2, 20);

	c->updateScreen();

	Beep(400, 500);
	Sleep(100);

	printf("It can easily be spread to others...\n");

	//draw red face spit virus towards green face

	for(int i = 200; i < 550; i += 15)
	{
		//draw red face and virus and then make beep sounds

		c->setBrushColor(255, 0, 0);

		drawFace(c, 200, c->canvasSizeY / 2, 20);
		drawVirus(c, i, c->canvasSizeY / 2, 5);

		c->setBrushColor(0, 255, 0);

		drawFaceReversed(c, 550, c->canvasSizeY / 2, 20);

		//update screen

		c->updateScreen();

		//beep

		Beep(250, 100);
		Sleep(50);
	}

	//draw 2 red faces and beep

	printf("THATS NOT GOOD!\n");

	c->setBrushColor(255, 0, 0);

	drawFace(c, 200, c->canvasSizeY / 2, 20);
	drawFaceReversed(c, 550, c->canvasSizeY / 2, 20);

	c->updateScreen();

	Beep(400, 500);
	Sleep(100);

	//sleep for a second

	Sleep(1000);

	//draw 3 exclaimation points

	drawExcalimation(c, 300, c->canvasSizeY / 2, 15);
	drawExcalimation(c, c->canvasSizeX / 2, c->canvasSizeY / 2, 15);
	drawExcalimation(c, 450, c->canvasSizeY / 2, 15);

	c->updateScreen();

	//make high pitched beep sound

	Beep(600, 500);
	Sleep(100);

	//draw light blue face

	printf("But... if he wears a mask (represented in blue)\n");
	c->setBrushColor(100, 200, 255);

	drawFace(c, c->canvasSizeX / 2, c->canvasSizeY / 2, 20);
	c->updateScreen();

	//beep n' sleep

	Beep(350, 500);
	Sleep(100);

	//draw virus go towards face

	for(int i = c->canvasSizeX; i > (c->canvasSizeX / 2) + 50; i -= 30)
	{
		//draw face

		c->setBrushColor(100, 200, 255);
		drawFace(c, c->canvasSizeX / 2, c->canvasSizeY / 2, 20);
		
		//draw virus

		c->setBrushColor(255, 64, 0);
		drawVirus(c, i, c->canvasSizeY / 2, 10);

		c->updateScreen();

		//beep and sleep

		Beep(200, 150);
		Sleep(50);
	}
	
	//shrink virus

	for(int i = 10; i > 0; i--)
	{
		//draw face

		c->setBrushColor(100, 200, 255);
		drawFace(c, c->canvasSizeX / 2, c->canvasSizeY / 2, 20);

		//draw virus

		c->setBrushColor(255, 64, 0);
		drawVirus(c, (c->canvasSizeX / 2) + 50, c->canvasSizeY / 2, i);

		c->updateScreen();

		//beep and sleep

		Beep(500 - ((10 - i) * 20), 100);
		Sleep(25);
	}

	//draw face alone

	c->setBrushColor(100, 200, 255);
	drawFace(c, c->canvasSizeX / 2, c->canvasSizeY / 2, 20);

	printf("He'll be safe!\n(int the following simulation, mask wearers only have a 5 percent chance of being infected)\n");

	c->updateScreen();

	Beep(300, 200);
	Sleep(25);

	//sleep for 1 second

	Sleep(3500);

	system("cls");
}

//SCENE FOUR

void SCENE_FOUR(Canvas* c)
{
	//people count
	const int PCOUNT = 25;

	//set background color
	c->setBackground(0, 0, 0);

	//clear screen
	system("cls");

	//give description

	printf("SIMULATING THE SPREAD\n");

	Beep(450, 500); //beep

	Sleep(1000);

	Human* hArr = new Human[PCOUNT]; //array of 20 humans

	//infection count

	int infectionCount = 1;

	//time iteration, for movement refining

	long moveTime = 0;

	printf("HEALTHY: %d\nINFECTED: %d\n", PCOUNT - infectionCount, infectionCount);

	//simulate humans

	hArr[0].HS = 1;
	hArr[0].G = 0;
	hArr[0].R = 255;

	hArr[0].MX = (rand() % 10) - 5;
	hArr[0].MY = (rand() % 10) - 5;

	//make 5 masked humans

	for(int i = 1; i < 6; i++)
	{
		hArr[i].HS = 2;

		//change color

		hArr[i].R = 100;
		hArr[i].G = 200;
		hArr[i].B = 255;
	}

	for (int i = 1; i < PCOUNT; i++) //place each human at random position
	{
		hArr[i].X = (rand() % c->canvasSizeX);
		hArr[i].Y = (rand() % c->canvasSizeY);

		hArr[i].MX = (rand() % 10) - 5;
		hArr[i].MY = (rand() % 10) - 5;
	}

	while (1) //forever loop
	{


		moveTime++;

		//update background so it gets redder every infection

		c->setBackground(infectionCount, 0, 0);

		for (int i = 0; i < PCOUNT; i++)
		{
			c->setBrushColor(hArr[i].R, hArr[i].G, hArr[i].B); //set color

			drawHuman(c, hArr[i].X, hArr[i].Y, 5); //draw human

			if (hArr[i].X > c->canvasSizeX) //check if human is out of bounds
			{
				hArr[i].X = 0;
			}

			if (hArr[i].X < 0) //check if human is out of bounds
			{
				hArr[i].X = c->canvasSizeX;
			}

			if (hArr[i].Y > c->canvasSizeY) //check if human is out of bounds
			{
				hArr[i].Y = 0;
			}

			if (hArr[i].Y < 0) //check if human is out of bounds
			{
				hArr[i].Y = c->canvasSizeY;
			}

			//update human position... do it faster if infected

			int r = (rand() % 5) + 5;

			if (moveTime % r == 0)
			{
				hArr[i].MX = (rand() % 16) - 8;
				hArr[i].MY = (rand() % 16) - 8;
			}

			hArr[i].X += hArr[i].MX;
			hArr[i].Y += hArr[i].MY;

			//some thing

			Human checkHuman = hArr[i];

			//check for collisions

			for (int j = 0; j < PCOUNT; j++)
			{
				if (checkHuman.X > hArr[j].X - 30 && checkHuman.X < hArr[j].X + 30) //check for X collision
				{
					if (checkHuman.Y > hArr[j].Y - 50 && checkHuman.Y < hArr[j].Y + 30) //check for Y collision
					{
						

						//check for infected

						if (hArr[j].HS == 1)
						{
							if (hArr[i].HS == 0)
							{
								infectionCount++;

								system("cls"); //clear

								printf("HEALTHY: %d\nINFECTED: %d\n", PCOUNT - infectionCount, infectionCount);

								//change color

								hArr[i].HS = true;
								hArr[i].G = 0;
								hArr[i].R = 255;
							}

							if (hArr[i].HS == 2)
							{
								int p = (rand() % 100); //p for percentage

								//printf("%d\tHuman Handle: %d\n", p, i); //DEBUG ONLYYYYYYYYYYYY

								if (p <= 2 && hArr[i].colliding == false) //2% chance
								{
									infectionCount++;

									system("cls"); //clear

									printf("HEALTHY: %d\nINFECTED: %d\n", PCOUNT - infectionCount, infectionCount);

									//change color

									hArr[i].HS = true;
									hArr[i].G = 0;
									hArr[i].B = 0;
									hArr[i].R = 255;

									hArr[i].colliding = true;
								}
							}
						}


					}
				}

				hArr[i].colliding = false;
			}

		}

		//check if everyone is infected

		if (infectionCount >= PCOUNT)
		{
			break;
		}

		if (GetKeyState(VK_RETURN) < -5) //skip with enter key
		{
			break;
		}

		c->updateScreen();

	}

	//cover screen in virus sprite

	for (int i = 0; i < c->canvasSizeX + 20; i += 100)
	{
		for (int j = 0; j < c->canvasSizeY + 20; j += 100)
		{
			c->setBrushColor(255, 128, 0);
			drawVirus(c, i, j, 10);
		}

		Sleep(100);
		Beep(300, 100);
		c->updateScreen(false);
	}

	//sleep

	Sleep(1000);
}

//SCENE 5

void SCENE_FIVE(Canvas* c)
{
	system("cls");

	c->updateScreen();

	c->setBackground(0, 0, 0);

	c->updateScreen();

	//draw 2 humans, 1 green 1 white
	
	c->setBrushColor(0, 255, 0);
	drawHuman(c, 200, c->canvasSizeY / 2, 30);

	c->setBrushColor(255, 255, 255);
	drawHuman(c, 550, c->canvasSizeY / 2, 30);

	printf("In the following simulation,\n doctors will be represented in white\n");

	c->updateScreen();

	Sleep(1000);

	printf("Doctors can help make humans immune to infection\n");

	//draw human 1 give human 2 a "vaccine" which is purple

	for(int i = 550; i > 200; i -= 25)
	{
		c->setBrushColor(0, 255, 0);
		drawHuman(c, 200, c->canvasSizeY / 2, 30);

		c->setBrushColor(255, 255, 255);
		drawHuman(c, 550, c->canvasSizeY / 2, 30);

		//draw vaccine moving

		c->setBrushColor(200, 75, 200); //purple
		drawVaccine(c, i, c->canvasSizeY / 2, 10);

		c->updateScreen();

		//beep and sleep

		Beep(400, 100);
		Sleep(75);
	}

	//make left human purple

	c->setBrushColor(200, 75, 200);
	drawHuman(c, 200, c->canvasSizeY / 2, 30);

	c->setBrushColor(255, 255, 255);
	drawHuman(c, 550, c->canvasSizeY / 2, 30);

	c->updateScreen();

	//beep n sleep

	Beep(500, 200);

	Sleep(1000);

	//draw 2 humans, 1 red 1 white

	c->setBrushColor(255, 0, 0);
	drawHuman(c, 200, c->canvasSizeY / 2, 30);

	c->setBrushColor(255, 255, 255);
	drawHuman(c, 550, c->canvasSizeY / 2, 30);

	c->updateScreen();

	Sleep(1000);

	printf("And can also cure sick humans\n");

	//draw human 1 give human 2 a "vaccine" which is purple

	for (int i = 550; i > 200; i -= 25)
	{
		c->setBrushColor(255, 0, 0);
		drawHuman(c, 200, c->canvasSizeY / 2, 30);

		c->setBrushColor(255, 255, 255);
		drawHuman(c, 550, c->canvasSizeY / 2, 30);

		//draw vaccine moving

		c->setBrushColor(200, 75, 200); //purple
		drawVaccine(c, i, c->canvasSizeY / 2, 10);

		c->updateScreen();

		//beep and sleep

		Beep(400, 100);
		Sleep(75);
	}

	//make left human purple

	c->setBrushColor(200, 75, 200);
	drawHuman(c, 200, c->canvasSizeY / 2, 30);

	c->setBrushColor(255, 255, 255);
	drawHuman(c, 550, c->canvasSizeY / 2, 30);

	c->updateScreen();

	//beep n sleep

	Beep(500, 200);

	Sleep(1000);



	system("cls");



	//covid simulation

	const char PCOUNT = 50;

	Human* hArr = new Human[PCOUNT]; //array of 20 humans

	//infection count

	int infectionCount = 1;

	//time iteration, for movement refining

	long moveTime = 0;

	printf("OF: %d PEOPLE...\n HEALTHY: %d\nINFECTED: %d\n", PCOUNT, PCOUNT - infectionCount, infectionCount);

	//simulate humans

	hArr[0].HS = 1;
	hArr[0].G = 0;
	hArr[0].R = 255;

	hArr[0].MX = (rand() % 10) - 5;
	hArr[0].MY = (rand() % 10) - 5;

	//make 12 masked humans

	for (int i = 1; i < 12; i++)
	{
		hArr[i].HS = 2;

		//change color

		hArr[i].R = 100;
		hArr[i].G = 200;
		hArr[i].B = 255;
	}

	//make 1 doctor

	hArr[10].R = 255;
	hArr[10].G = 255;
	hArr[10].B = 255;
	hArr[10].HS = 4;

	for (int i = 1; i < PCOUNT; i++) //place each human at random position
	{
		hArr[i].X = (rand() % c->canvasSizeX);
		hArr[i].Y = (rand() % c->canvasSizeY);

		hArr[i].MX = (rand() % 10) - 5;
		hArr[i].MY = (rand() % 10) - 5;
	}

	while (1) //forever loop
	{


		moveTime++;

		//update background so it gets redder every infection

		c->setBackground(infectionCount, 0, 0);

		for (int i = 0; i < PCOUNT; i++)
		{
			c->setBrushColor(hArr[i].R, hArr[i].G, hArr[i].B); //set color

			drawHuman(c, hArr[i].X, hArr[i].Y, 5); //draw human

			if (hArr[i].X > c->canvasSizeX) //check if human is out of bounds
			{
				hArr[i].X = 0;
			}

			if (hArr[i].X < 0) //check if human is out of bounds
			{
				hArr[i].X = c->canvasSizeX;
			}

			if (hArr[i].Y > c->canvasSizeY) //check if human is out of bounds
			{
				hArr[i].Y = 0;
			}

			if (hArr[i].Y < 0) //check if human is out of bounds
			{
				hArr[i].Y = c->canvasSizeY;
			}

			//update human position... do it faster if infected

			int r = (rand() % 5) + 5;

			if (moveTime % r == 0)
			{
				hArr[i].MX = (rand() % 16) - 8;
				hArr[i].MY = (rand() % 16) - 8;
			}

			hArr[i].X += hArr[i].MX;
			hArr[i].Y += hArr[i].MY;

			//some thing

			Human checkHuman = hArr[i];

			//check for collisions

			for (int j = 0; j < PCOUNT; j++)
			{
				if (checkHuman.X > hArr[j].X - 30 && checkHuman.X < hArr[j].X + 30) //check for X collision
				{
					if (checkHuman.Y > hArr[j].Y - 50 && checkHuman.Y < hArr[j].Y + 30) //check for Y collision
					{
						//check for doctor

						if(hArr[j].HS == 4 && hArr[i].HS != 4)
						{
							//check if self is infected

							if(hArr[i].HS == 1)
							{
								infectionCount -= 1;

								system("cls"); //clear

								printf("OF: %d PEOPLE...\n HEALTHY: %d\nINFECTED: %d\n", PCOUNT, PCOUNT - infectionCount, infectionCount);
							}

							hArr[i].HS = 3;

							hArr[i].R = 200;
							hArr[i].G = 75;
							hArr[i].B = 200;


						}

						//check for infected

						if (hArr[j].HS == 1)
						{
							if (hArr[i].HS == 0)
							{
								infectionCount++;

								system("cls"); //clear

								printf("OF: %d PEOPLE...\n HEALTHY: %d\nINFECTED: %d\n", PCOUNT, PCOUNT - infectionCount, infectionCount);

								//change color

								hArr[i].HS = true;
								hArr[i].G = 0;
								hArr[i].R = 255;
							}

							if (hArr[i].HS == 2)
							{
								int p = (rand() % 100); //p for percentage

								//printf("%d\tHuman Handle: %d\n", p, i); //DEBUG ONLYYYYYYYYYYYY

								if (p <= 2 && hArr[i].colliding == false) //2% chance
								{
									infectionCount++;

									system("cls"); //clear

									printf("OF: %d PEOPLE...\n HEALTHY: %d\nINFECTED: %d\n", PCOUNT, PCOUNT - infectionCount, infectionCount);

									//change color

									hArr[i].HS = true;
									hArr[i].G = 0;
									hArr[i].B = 0;
									hArr[i].R = 255;

									hArr[i].colliding = true;
								}
							}
						}


					}
				}

				hArr[i].colliding = false;
			}

		}

		//check if everyone is infected

		if (infectionCount >= PCOUNT)
		{
			break;
		}

		if (GetKeyState(VK_RETURN) < -5) //skip with enter key
		{
			break;
		}

		c->updateScreen();

	}
}


//the following is misc functions


int attachConsoleToWindow(SDL_Window* w)
{
	//get window handle of i/o console

	HWND win = GetConsoleWindow();

	SetWindowTextA(win, " ");

	int winX; //set window X
	int winY; //set window Y

	int offsetX;
	int offsetY;

	SDL_GetWindowPosition(w, &winX, &winY); // get window pos from W
	SDL_GetWindowSize(w, &offsetX, &offsetY);

	//increment by size

	winX -= 300;

	//set pos of console window

	SetWindowPos(win, NULL, winX, winY, 300, 500, NULL);

	return 0;
}

//draw scaleable line

int drawLineS(Canvas* c, int X1, int Y1, int X2, int Y2, int offsetX, int offsetY, float scale)
{
	c->drawLine(X1 * scale + offsetX, -Y1 * scale + offsetY, X2 * scale + offsetX, -Y2 * scale + offsetY);

	return 0;
}

//draw virus

int drawVirus(Canvas* c, int X, int Y, float S)
{
	//outer ring

	drawLineS(c, 0, -4, 2, -3, X, Y, S);
	drawLineS(c, 2, -3, 4, 0, X, Y, S);
	drawLineS(c, 4, 0, 2, 3, X, Y, S);
	drawLineS(c, 2, 3, 0, 4, X, Y, S);
	drawLineS(c, 0, 4, -2, 3, X, Y, S);
	drawLineS(c, -2, 3, -4, 0, X, Y, S);
	drawLineS(c, -4, 0, -2, -3, X, Y, S);
	drawLineS(c, -2, -3, 0, -4, X, Y, S);

	//inner ring

	drawLineS(c, 0, -2, 1, -1, X, Y, S);
	drawLineS(c, 1, -1, 1, 1, X, Y, S);
	drawLineS(c, 1, 1, 0, 2, X, Y, S);
	drawLineS(c, 0, 2, -1, 1, X, Y, S);
	drawLineS(c, -1, 1, -1, -1, X, Y, S);
	drawLineS(c, -1, -1, 0, -2, X, Y, S);

	//draw virus "sprongs"

	drawLineS(c, 4, 0, 5, 0, X, Y, S);
	drawLineS(c, 2, 3, 3, 4, X, Y, S);
	drawLineS(c, 0, 4, 0, 5, X, Y, S);
	drawLineS(c, -2, 3, -3, 4, X, Y, S);
	drawLineS(c, -4, 0, -5, 0, X, Y, S);
	drawLineS(c, -2, -3, -3, -4, X, Y, S);
	drawLineS(c, 2, -3, 3, -4, X, Y, S);
	drawLineS(c, 0, -4, 0, -5, X, Y, S);

	//optional sprong heads

	drawLineS(c, -1, -5, 1, -5, X, Y, S);
	drawLineS(c, 2, -5, 4, -3, X, Y, S);
	drawLineS(c, 4, 3, 2, 5, X, Y, S);
	drawLineS(c, 1, 5, -1, 5, X, Y, S);
	drawLineS(c, 5, 1, 5, -1, X, Y, S);
	drawLineS(c, -5, 1, -5, -1, X, Y, S);
	drawLineS(c, -4, 3, -2, 5, X, Y, S);
	drawLineS(c, -4, -3, -2, -5, X, Y, S);

	return 0;

}

//draw human

#define d X, Y, S

void drawHuman(Canvas* c, int X, int Y, float S)
{
	//draw head

	drawLineS(c, 1 , 1 , 2, 2, d);
	drawLineS(c, 2, 2, 2, 4, d);
	drawLineS(c, 2, 4, 1, 5, d);
	drawLineS(c, 1, 5, -1, 5, d);
	drawLineS(c, -1, 1, -2, 2, d);
	drawLineS(c, -2, 2, -2, 4, d);
	drawLineS(c, -2, 4, -1, 5, d);

	//left side of body

	drawLineS(c, 1, 1, 4, 1, d);
	drawLineS(c, 4, 1, 4, 0, d);
	drawLineS(c, 4, 0, 2, 0, d);
	drawLineS(c, 2, 0, 2, -3, d);
	drawLineS(c, 2, -3, 3, -7, d);
	drawLineS(c, 3, -7, 2, -7, d);
	drawLineS(c, 2, -7, 0, -4, d);

	//right side

	drawLineS(c, -1, 1, -4, 1, d);
	drawLineS(c, -4, 1, -4, 0, d);
	drawLineS(c, -4, 0, -2, 0, d);
	drawLineS(c, -2, 0, -2, -3, d);
	drawLineS(c, -2, -3, -3, -7, d);
	drawLineS(c, -3, -7, -2, -7, d);
	drawLineS(c, -2, -7, -0, -4, d);



}

//draw face

void drawFace(Canvas* c, int X, int Y, float S)
{
	drawLineS(c, 2, -5, 2, -2, d);
	drawLineS(c, 2, -2, 5, -2, d);
	drawLineS(c, 5, -2, 5, 1, d);
	drawLineS(c, 5, 1, 6, 1, d);
	drawLineS(c, 6, 1, 4, 4, d);
	drawLineS(c, 4, 4, 5, 5, d);
	drawLineS(c, 5, 5, 3, 7, d);
	drawLineS(c, 3, 7, -3, 7, d);
	drawLineS(c, -3, 7, -5, 4, d);
	drawLineS(c, -5, 4, -5, 1, d);
	drawLineS(c, -5, 1, -3, -2, d);
	drawLineS(c, -3, -2, -3, -5, d);

}

void drawFaceReversed(Canvas* c, int X, int Y, float S)
{
	drawLineS(c, -2, -5, -2, -2, d);
	drawLineS(c, -2, -2, -5, -2, d);
	drawLineS(c, -5, -2, -5, 1, d);
	drawLineS(c, -5, 1, -6, 1, d);
	drawLineS(c, -6, 1, -4, 4, d);
	drawLineS(c, -4, 4, -5, 5, d);
	drawLineS(c, -5, 5, -3, 7, d);
	drawLineS(c, -3, 7, 3, 7, d);
	drawLineS(c, 3, 7, 5, 4, d);
	drawLineS(c, 5, 4, 5, 1, d);
	drawLineS(c, 5, 1, 3, -2, d);
	drawLineS(c, 3, -2, 3, -5, d);
}

//draw exclaimiation

void drawExcalimation(Canvas* c, int X, int Y, float S)
{
	//bottom point

	drawLineS(c, 1, -5, 1, -2, d);
	drawLineS(c, 1, -2, -2, -2, d);
	drawLineS(c, -2, -2, -2, -5, d);
	drawLineS(c, -2, -5, 1, -5, d);

	//top line

	drawLineS(c, 1, 0, 1, 10, d);
	drawLineS(c, 1, 10, -2, 10, d);
	drawLineS(c, -2, 10, -2, 0, d);
	drawLineS(c, -2, 0, 1, 0, d);
}

//draw vaccine

void drawVaccine(Canvas* c, int X, int Y, float S)
{
	drawLineS(c, 1, -4, 1, -2, d);
	drawLineS(c, 1, -2, 3, -2, d);
	drawLineS(c, 3, -2, 3, 1, d);
	drawLineS(c, 3, 1, 1, 1, d);
	drawLineS(c, 1, 1, 1, 3, d);
	drawLineS(c, 1, 3, -2, 3, d);

	drawLineS(c, -2, 3, -2, 1, d);
	drawLineS(c, -2, 1, -4, 1, d);
	drawLineS(c, -4, 1, -4, -2, d);
	drawLineS(c, -4, -2, -2, -2, d);
	drawLineS(c, -2, -2, -2, -4, d);
	drawLineS(c, -2, -4, 1, -4, d);
	
}

//human constructor

Human::Human()
{
	this->B = 0;
	this->R = 0;
	this->G = 255;

	this->infected = 0;

	this->X = 0;
	this->Y = 0;

	this->MX = 0;
	this->MY = 0;

	this->HS = 0;
}