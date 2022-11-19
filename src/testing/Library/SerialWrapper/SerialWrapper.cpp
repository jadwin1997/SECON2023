#include "SerialWrapper.h"

SerialWrapper::SerialWrapper()
{

}

bool SerialWrapper::start()
{
  Serial.begin(9600);
  Serial.setTimeout(1);
}

void SerialWrapper::stop()
{
  Serial.end();
}

int SerialWrapper::read()
{
  return Serial.readString().toInt();
}

void SerialWrapper::write(int x)
{
  Serial.print(x);
}
