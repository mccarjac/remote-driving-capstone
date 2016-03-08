#include <iostream>

#include "LogitechControllerSDK\LogiControllerInput.h"
#include "LogitechControllerSDK\LogiWheel.h"
#include "wheelApi.h"

using namespace LogitechControllerInput;
using namespace LogitechSteeringWheel;
using namespace std;

int main(int argc, char** argv) {
	// Create the wheelApi object to get controller input
	wheelApi w;
	while (!w.isConnected()) {
		cout << "No controller connected" << endl;
		exit(-1);
	}

	while (true) {
		wheelInputs a = w.getAll();
		system("cls");
		a.print();
		Sleep(50);
	}

	// Open connection to vehicle

	// Start stream wheel data

	// Start recieving and displaying video
}