#include "Arduino.h"

#ifndef MOTOR_H
#define MOTOR_H


class Motor {
  private:
  //! The positive pin.
  uint8_t posPin;
  //! The negative pin.
  uint8_t negPin;
  //! The pwm pin.
  uint8_t pwmPin;

  //! The current speed of the motor, as an 8bit integer.
  uint8_t speed;
  //! The max speed of the motor, as an 8bit integer.
  uint8_t maxSpeed;

  //! Whether or not the motor is currently in reverse.
  boolean isInReverse;
  
  public:
  //! Default constructor. Should not be used.
  Motor();

  /**
   * Constructs a motor object.
   *
   * @param posPin the positive pin
   * @param negPin the negative pin
   * @param pwmPin the pwm pin
   */
  Motor(uint8_t posPin, uint8_t negPin, uint8_t pwmPin);

  /**
   * Constructs a motor object.
   *
   * @param posPin the positive pin
   * @param negPin the negative pin
   * @param pwmPin the pwm pin
   * @param maxSpeed the max speed of the motor
   */
  Motor(uint8_t posPin, uint8_t negPin, uint8_t pwmPin, uint8_t maxSpeed);

  //! The motor's setup function. Should run in setup().
  void setup();
  //! The motor's loop function. Should run in loop().
  void loop();

  /**
   * Sets the motor speed with an input from -500 to 500. -500 indicates full speed reverse, 
   * 500 indicates full speed forward, 0 indicates stop.
   *
   * @param speed an integer from -500 to 500
   */
  void setSpeed(int speed);

  /**
   * Sets the motor speed with an input from 0 to 255 and whether or not to reverse.
   * 0 indicates stop, 255 indicates full speed.
   *
   * @param speed an integer from 0-255
   * @param reverse true if the motor should reverse, false else
   */
  void setSpeed(uint8_t speed, boolean reverse);
};

#endif // MOTOR_H
