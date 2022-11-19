#include "Arduino.h"
#include "Motor.h"

Motor::Motor()
{
  Motor(255, 255, 255);
}

Motor::Motor(uint8_t posPin, uint8_t negPin, uint8_t pwmPin)
{
  Motor(posPin, negPin, pwmPin, 100);
}

Motor::Motor(uint8_t posPin, uint8_t negPin, uint8_t pwmPin, uint8_t maxSpeed)
{
  this->posPin = posPin;
  this->negPin = negPin;
  this->pwmPin = pwmPin;

  speed = 0;
  this->maxSpeed = maxSpeed;
  isInReverse = false;
}

void Motor::setup()
{
  pinMode(posPin, OUTPUT);
  pinMode(negPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);

  // stopped by default
  analogWrite(pwmPin, 0);

  // set to forward by default
  digitalWrite(posPin, HIGH);
  digitalWrite(negPin, LOW);
}

void Motor::loop()
{
  analogWrite(pwmPin, speed);
}

void Motor::setSpeed(int speed)
{
  // we have to constrain the value incase it's over 500 or under -500
  if(speed < 0)
  {
    setSpeed(map(constrain(speed, -500, 0), -500, 0, 0, 255), true); // todo: define 500 as constants in file, not here
  }
  else 
  {
    setSpeed(map(constrain(speed, 0, 500), 0, 500, 0, 255), false);
  }
}

void Motor::setSpeed(uint8_t speed, boolean reverse)
{
  this->speed = map(speed, 0, 255, 0, maxSpeed);
  if(isInReverse != reverse)
  {
    if(reverse)
    {
      digitalWrite(posPin, LOW);
      digitalWrite(negPin, HIGH);
    }
    else 
    {
      digitalWrite(posPin, HIGH);
      digitalWrite(negPin, LOW);
    }
  }
}