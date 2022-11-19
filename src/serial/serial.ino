#include <ArduinoJson.h>

String read;
StaticJsonDocument<64> doc;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(0.5);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  while(!Serial.available());
  read = Serial.readString();
  DeserializationError error = deserializeJson(doc, read);
  int time = doc["time"];
  Serial.write(time);
}
