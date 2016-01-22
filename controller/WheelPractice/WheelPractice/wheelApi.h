#ifndef WHEELAPI
#define WHEELAPI

#include "LogitechControllerSDK\LogiControllerInput.h"
#include "LogitechControllerSDK\LogiWheel.h"

using namespace LogitechControllerInput;
using namespace LogitechSteeringWheel;

class wheelApi {
public:
	wheelApi();
	bool* getButtons();
	int getWheel();
	int getGas();
	int getBreaks();
	int getDpad();
	int getShifter();
private:
	ControllerInput* g_controllerInput;
	Wheel* g_wheel;
	bool* buttons;
	int wheel;
	int gas;
	int breaks;
	int dpad;
	void update();
	void timer();
};

#endif