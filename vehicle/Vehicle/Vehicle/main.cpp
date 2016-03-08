#include "bbb.h"

int main() {
    BBBApi* bbb = new BBBApi();
    wheelInputs ins;
    ins.buttons = new bool[23];
    while (true) {
        for (int i = 0;i < 23;i++) {
            ins.buttons[i] = false;
        }
        
        cout << "Enter gas: ";
        cin >> ins.gas;

        cout << "Enter dpad: ";
        cin >> ins.dpad;
        
        cout << "Enter brakes: ";
        cin >> ins.brakes;

        cout << "Enter wheel: ";
        cin >> ins.wheel;

        bbb->Send(ins);
        char choice;
        cout << "Send another: ";
        cin >> choice;

        if (choice == 'n')
            break;
    }

    delete ins.buttons;
    delete bbb;
    return 0;
}