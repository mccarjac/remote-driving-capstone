#ifndef WHEELAPI
#define WHEELAPI

#include <iostream>
#include <windows.h>

#include "LogitechControllerSDK\LogiControllerInput.h"
#include "LogitechControllerSDK\LogiWheel.h"

using namespace std;
using namespace LogitechControllerInput;
using namespace LogitechSteeringWheel;

struct wheelInputs {
	bool* buttons;
	int wheel;
	int gas;
	int brakes;
	int dpad;
	void print() {
		for (int i = 0;i < 23;i++) {
			cout << "Button " << i << ": " << buttons[i] << endl;
		}
		cout << "Wheel: " << wheel << endl;
		cout << "Gas: " << gas << endl;
		cout << "Brakes: " << brakes << endl;
		cout << "Dpad: " << dpad << endl;
	}
};

class wheelApi {
public:
	wheelApi();
	~wheelApi();
	bool* getButtons();
	int getWheel();
	int getGas();
	int getBrakes();
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
	int brakes;
	int dpad;
};
#endif