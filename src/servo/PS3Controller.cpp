#include "PS3Controller.h"

PS3Controller::PS3Controller() : Btd(&Usb), PS3(&Btd)
{
    connected = false;
}


bool PS3Controller::isConnected()
{
    return connected;
}


void PS3Controller::updateControllerState()
{
    // add whatever here
    controller_state[0] = PS3.getAnalogHat(LeftHatY);
}


uint8_t* PS3Controller::getControllerState()
{
    return controller_state;
}


uint8_t PS3Controller::getControllerStateSize()
{
    return STATE_ARRAY_SIZE;
}

bool PS3Controller::setupPS3Controller()
{
    if (Usb.Init() == -1)
    {
        Serial.println("OSC did not start");
        return false;
    }
    Serial.println("Bluetooth ready");
    return true;
}


void PS3Controller::loopTask()
{
    Usb.Task();
    if(PS3.PS3Connected)
    {
        connected = true;
        updateControllerState();
    }
    else
    {
        connected = false;
    }
}

