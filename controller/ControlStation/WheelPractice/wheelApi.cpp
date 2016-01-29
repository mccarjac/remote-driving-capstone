#include "wheelApi.h"

// Timer function used to update the class members
DWORD WINAPI timer(LPVOID lpParam) {
	wheelApi* w = (wheelApi*)lpParam;

	while (true) {
		w->update();
		Sleep(30);
	}
	return 0;
}

// Default constructor that connects the wheel and sets up the buttons array
wheelApi::wheelApi() {
	// Connect to wheel
	g_controllerInput = new ControllerInput(GetConsoleWindow(), true);
	g_wheel = new Wheel(g_controllerInput);

	// Setup buttons
	buttons = new bool[23];
	for (int i = 0; i < 23; i++) {
		buttons[i] = false;
	}

	// Create the mutex
	mutex = CreateMutex(NULL, false, NULL);

	// Create the timer thread
	threadHandle = CreateThread(0, 0, timer, this, 0, &threadId);
}

// Default deconstructor
// Deallocates all pointers and terminates the timer thread
wheelApi::~wheelApi() {
	delete buttons;
	delete g_wheel;
	delete g_controllerInput;
	TerminateThread(threadHandle, NULL);
	CloseHandle(mutex);
	CloseHandle(threadHandle);
}

// Returns the array of bools that indicate if a button is pressed
bool* wheelApi::getButtons() {
	DWORD waitResult = WaitForSingleObject(mutex, 100);
	bool* b = buttons;
	ReleaseMutex(mutex);

	return b;
}

// Returns the int value of the gas
// 32676 is not pressed
// -32678 is fully pressed 
int wheelApi::getGas() {
	DWORD waitResult = WaitForSingleObject(mutex, 100);
	int g = gas;
	ReleaseMutex(mutex);

	return g;
}

// Returns the int value of the wheel
// 32676 is fully right
// -32678 is fully left
int wheelApi::getWheel() {
	DWORD waitResult = WaitForSingleObject(mutex, 100);
	int w = wheel;
	ReleaseMutex(mutex);

	return w;
}
// Returns the int value of the breaks
// 32676 is not pressed
// -32678 is fully pressed
int wheelApi::getBreaks() {
	DWORD waitResult = WaitForSingleObject(mutex, 100);
	int b = breaks;
	ReleaseMutex(mutex);

	return b;
}

// Returns the int value of the dpad
// Range is 0 - 31999
// Up is 0 then value increases lineraly as you go clockwise
// ie right is 9000, down is 18000, and left is 27000
int wheelApi::getDpad() {
	DWORD waitResult = WaitForSingleObject(mutex, 100);
	int d = dpad;
	ReleaseMutex(mutex);

	return d;
}

// Returns the current state of the shifter as an int
// Based on button mapping we can map buttons to certain gears
int wheelApi::getShifter() {
	DWORD waitResult = WaitForSingleObject(mutex, 100);
	int s;
	if (buttons[8])
		s = 1;
	else if (buttons[9])
		s = 2;
	else if (buttons[10])
		s = 3;
	else if (buttons[11])
		s = 4;
	else if (buttons[12])
		s = 5;
	else if (buttons[13])
		s = 6;
	else
		s = 0;
	ReleaseMutex(mutex);

	return s;
}

// Pulls data from the wheel and updates the class members
void wheelApi::update() {
	DWORD waitResult = WaitForSingleObject(mutex, 100);
	
	g_controllerInput->Update();
	g_wheel->Update();

	for (int i = 0; i < 23; i++) {
		if (g_wheel->ButtonTriggered(0, i))
			buttons[i] = true;
		else if (g_wheel->ButtonReleased(0, i))
			buttons[i] = false;
	}

	// check for wheel turn
	wheel = g_wheel->GetState(0)->lX;

	// Check gas
	gas = g_wheel->GetState(0)->lY;

	// Check break
	breaks = g_wheel->GetState(0)->lRz;

	// Check Dpad
	dpad = g_wheel->GetState(0)->rgdwPOV[0];

	ReleaseMutex(mutex);
}

