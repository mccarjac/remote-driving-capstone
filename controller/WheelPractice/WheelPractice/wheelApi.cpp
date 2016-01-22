#include "wheelApi.h"

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

	// Start pulling the input on a timer
	timer();
}

// Returns the array of bools that indicate if a button is pressed
bool* wheelApi::getButtons() {
	return buttons;
}

// Returns the int value of the gas
// 32676 is not pressed
// -32678 is fully pressed 
int wheelApi::getGas() {
	return gas;
}

// Returns the int value of the wheel
// 32676 is fully right
// -32678 is fully left
int wheelApi::getWheel() {
	return wheel;
}
// Returns the int value of the breaks
// 32676 is not pressed
// -32678 is fully pressed
int wheelApi::getBreaks() {
	return breaks;
}

// Returns the int value of the dpad
// Range is 0 - 31999
// Up is 0 then value increases lineraly as you go clockwise
// ie right is 9000, down is 18000, and left is 27000
int wheelApi::getDpad() {
	return dpad;
}

// Returns the current state of the shifter as an int
// Based on button mapping we can map buttons to certain gears
int wheelApi::getShifter() {
	if (buttons[8])
		return 1;
	if (buttons[9])
		return 2;
	if (buttons[10])
		return 3;
	if (buttons[11])
		return 4;
	if (buttons[12])
		return 5;
	if (buttons[13])
		return 6;

	return 0;
}

// Pulls data from the wheel and updates the class members
void wheelApi::update() {
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
}

// Updates the class members and then sleeps before repeating
void wheelApi::timer() {
	while (true) {
		update();
		Sleep(10);
	}
}