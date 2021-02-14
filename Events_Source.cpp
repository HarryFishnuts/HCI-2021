#include "BUGL_Events.h"

#include<stdio.h>
#include<Windows.h>
#include<WinUser.h>

#define EXECUTE_ONCE true
#define LOOP false
	
long Event::generateEventHandle(const char* eventName)
{
	char charBuffer[64];

	int i = 0;
	char c;
	while (1)
	{
		c = eventName[i];

		//check for valid name... a - z & A - Z no numbers... also accepts '_' and '' and - 0 9

		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == ' ' || (c >= '0' && c <= '9'))
		{
			charBuffer[i] = c;
			i++;
		}
		else
		{
			long toReturn = 0;

			for (int j = 0; j < i; j++)
			{
				toReturn += ((int)charBuffer[j] * (int)charBuffer[j] * j); //seed for eventHandle is existing handle + (ascii^2) * ascii index
			}

			toReturn *= i; //multiply by text size

			return toReturn;
		}
	}
}

//constructor

Event::Event(const char* _eventName, void (*function)(void), bool _eventType, int _callTime)
{
	eventHandle = generateEventHandle(_eventName);
	eventType = _eventType;
	callTime = _callTime;

	eventFunction = function;
}

//defualt constructor

Event::Event()
{
	eventHandle = -1;
	eventType = NULL;
	callTime = NULL;
	eventFunction = NULL;
}

//print handle

void Event::printEventHandle()
{
	printf("%ld\n", eventHandle);
}

//clock constructor

Clock::Clock(int _timeStep)
{
	timeSinceAwake = 0;
	timeStep = _timeStep;
}

//clock update

void Clock::cycleClock()
{
	Sleep(timeStep);

	timeSinceAwake++;

	if (timeSinceAwake > 1048576)
	{
		timeSinceAwake = 0;
	}
}

//constuctor for event handler

EventHandler::EventHandler()
{
	for (int i = 0; i < maxEventBufferSize; i++)
	{
		Event* temp = &eventBuffer[i];

		temp->eventFunction = NULL; //make each event point equal to NULL
	}
}

//register event

void EventHandler::registerEvent(void (*_function)(void), const char* eventHandle, bool eventType, int eventDelayTime)
{
	Event* _eventToRegister = new Event(eventHandle, _function, eventType, eventDelayTime); //create new event

	if (eventDelayTime == 0)
	{
		_eventToRegister->callTime = 1;
	}

	if (eventType == EXECUTE_ONCE)
	{
		_eventToRegister->callTime += timeSinceAwake;
	}

	for (int i = 0; i < maxEventBufferSize; i++)
	{
		Event* tempEvent = &eventBuffer[i];

		if (tempEvent->eventHandle == _eventToRegister->eventHandle)
		{
			eventBuffer[i] = *_eventToRegister; //dereference

			return;
		}

		if (tempEvent->eventFunction == NULL)
		{
			eventBuffer[i] = *_eventToRegister; //dereference

			return;
		}
	}

	//if for loop is passed, then it means buffer ran out of space

	system("cls");
	printf("EVENTBUFFER RAN OUT OF SPACE!!!\n");
	exit(0);
}

//remove event

int EventHandler::removeEvent(const char* eventHandle) //returns 0 if destroyed, returns 1 if not found
{
	//create handle

	Event* tempEvent = new Event(eventHandle, NULL, NULL, NULL);

	//get handle

	long handleToSearch = tempEvent->eventHandle;

	//search and destroy

	for (int i = 0; i < maxEventBufferSize; i++)
	{
		Event* searchEvent = &eventBuffer[i];

		if (searchEvent->eventHandle == handleToSearch)
		{
			searchEvent->eventHandle = -1;
			searchEvent->eventFunction = NULL;

			return 0;
		}
	}

	//if you got here, it means that nothing was found

	system("cls");
	printf("NO EVENT WITH NAME: <%s> WAS FOUND\n", eventHandle);
	return 1;
}

//checks for events that are to be activated

void EventHandler::checkForEvents()
{
	cycleClock(); //cycle clock
	//printf("%d\n", timeSinceAwake);

	//loop for events to execute

	for (int i = 0; i < maxEventBufferSize; i++)
	{
		Event* searchEvent = &eventBuffer[i]; //search event

		//makes sure searchEvent is not null

		if (searchEvent->eventFunction != NULL)
		{
			//what to do if eventType is EXECUTE_ONCE

			if (searchEvent->eventType == EXECUTE_ONCE) //if value is true it means that it's a single execute function
			{
				if (searchEvent->callTime == timeSinceAwake) //if value is less than time since awake
				{
					searchEvent->eventFunction(); //execute function

					//clear searchEvent

					searchEvent->eventHandle = -1;
					searchEvent->eventFunction = NULL;
				}
			}

			//what to do if eventType is LOOP

			if (searchEvent->eventType == LOOP)
			{
				if ((timeSinceAwake % searchEvent->callTime) == 0)
				{
					searchEvent->eventFunction();
				}
			}
		}
	}
}

//get event function

Event EventHandler::getEvent(const char* eventName)
{
	//create handle

	Event* tempEvent = new Event(eventName, NULL, NULL, NULL);

	//get handle

	long eventHandle = tempEvent->eventHandle;

	//search for handle

	Event* searchEvent;
	for (int i = 0; i < maxEventBufferSize; i++)
	{
		searchEvent = &eventBuffer[i];

		if (searchEvent->eventHandle == eventHandle)
		{
			return *searchEvent; //dereferenced
		}
	}
}

//function for getting time

long EventHandler::getCurrentTime()
{
	const long returnTime = timeSinceAwake;
	return returnTime;
}