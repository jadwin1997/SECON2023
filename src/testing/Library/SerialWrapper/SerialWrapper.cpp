#include "SerialWrapper.h"

SerialWrapper::SerialWrapper()
{

}

bool SerialWrapper::start()
{
    Serial.begin(9600);
}

void SerialWrapper::stop()
{
    Serial.end();
}


