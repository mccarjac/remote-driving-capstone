#include <iostream>

#include "mySocket.h"
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

    wheelInputs a;

	// Open connection to vehicle
    mySocket soc;
	// Start stream wheel data
    while (true) {
        a = w.getAll();
        soc.Send(a);
        Sleep(1000);
    }
	// Start recieving and displaying video
}