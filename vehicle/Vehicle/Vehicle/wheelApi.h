#ifndef WHEELAPI
#define WHEELAPI

using namespace std;

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
#endif