#include <iostream>
#include "LogitechControllerSDK\LogiControllerInput.h"
#include "LogitechControllerSDK\LogiWheel.h"

using namespace LogitechControllerInput;
using namespace LogitechSteeringWheel;

int main(int argc, char** argv) {
	ControllerInput* g_controllerInput = new ControllerInput(GetConsoleWindow(), true);
	Wheel* g_wheel = new Wheel(g_controllerInput);
	
	bool* b = new bool[23];
	for (int i = 0; i < 23; i++){
		b[i] = false;
	}
	int w;

	while (true) {
		g_controllerInput->Update();
		g_wheel->Update();
		system("cls");
		std::cout << "Connected: " << g_wheel->IsConnected(0) << std::endl;
		
		//Check buttons
		for (int i = 0; i < 23; i++) {
			std::cout << "Button " << i << ": ";
			if (g_wheel->ButtonTriggered(0, i))
				b[i] = true;
			else if (g_wheel->ButtonReleased(0, i))
				b[i] = false;
			std::cout << b[i] << std::endl;
		}

		//check for wheel turn
		w = g_wheel->GetState(0)->lX;
		std::cout << "Wheel: " << w << std::endl;

		//Check gas
		w = g_wheel->GetState(0)->lY;
		std::cout << "Gas: " << w << std::endl;

		//Check break
		w = g_wheel->GetState(0)->lRz;
		std::cout << "Break: " << w << std::endl;

		Sleep(100);
	}
}