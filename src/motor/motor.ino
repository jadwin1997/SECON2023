#include "Motor.h"

Motor motor1(8, 7, 3);
Motor motor2(4, 2, 11);

void setup() {
  // put your setup code here, to run once:
  motor1.setup();
  motor2.setup();
  motor1.setSpeed(500);
  motor2.setSpeed(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  motor1.loop();
  motor2.loop();
}
