#include "Motor.h"

Motor motor1(0, 1, 2);

void setup() {
  // put your setup code here, to run once:
  motor1.setup();
  motor1.setSpeed(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  motor1.loop();
}
