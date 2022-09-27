#include <Wire.h>
#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  AFMS.begin();
  motor1->setSpeed(255);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  motor1->run(FORWARD);
}

