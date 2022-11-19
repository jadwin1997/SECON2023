#include "SerialWrapper.h"

int read;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  while(!Serial.available());
  read = Serial.readString().toInt();
  Serial.print(read * 2);
}
