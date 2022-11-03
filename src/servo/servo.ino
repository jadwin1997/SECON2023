#include "Servo.h"
#include "PS3Controller.h"

Servo servo1(1);
PS3Controller ps3;

void setup() {
  // put your setup code here, to run once:
  servo1.setupServo();
  servo1.setTarget(0);
  ps3.setupPS3Controller();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  servo1.moveToTarget();
  ps3.loopTask();

  if(ps3.isConnected())
  {
      // if controller is connected, use values from controller
      servo1.setTarget(ps3.getControllerState()[0]);
  }
  else
  {
      // if controller not connected, use default values
      servo1.setTarget({0});
  }
}
