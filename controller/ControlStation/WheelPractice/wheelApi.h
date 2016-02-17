#ifndef WHEELAPI
#define WHEELAPI

#include <iostream>
#include <windows.h>

#include "LogitechControllerSDK\LogiControllerInput.h"
#include "LogitechControllerSDK\LogiWheel.h"

using namespace LogitechControllerInput;
using namespace LogitechSteeringWheel;

struct wheelInputs {
	bool* buttons;
	int wheel;
	int gas;
	int breaks;
	int dpad;
};

class wheelApi {
public:
	wheelApi();
	~wheelApi();
	bool* getButtons();
	int getWheel();
	int getGas();
	int getBreaks();
	int getDpad();
	int getShifter();
	bool isConnected();
	void update();
	wheelInputs getAll();
private:
	ControllerInput* g_controllerInput;
	Wheel* g_wheel;
	DWORD threadId;
	HANDLE threadHandle;
	HANDLE mutex;
	bool* buttons;
	int wheel;
	int gas;
	int breaks;
	int dpad;
};
#endif