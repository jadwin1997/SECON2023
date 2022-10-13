/**
 * @file Servo.h
 * @brief Controls an individual servo.
 */

#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

#ifndef SERVO_H
#define SERVO_H


class Servo {
    private:
    //! Stores the port of the servo.
    uint8_t port;

    //! Stores the servo's current target.
    uint8_t target;

    //! Minimum position of servo, 0.0 to 1.0.
    float min;

    //! Maximum position of servo, 0.0 to 1.0.
    float max;

    uint8_t offset;

    //! The PWM driver.
    Adafruit_PWMServoDriver pwm;

    public:
    //! Constructs a new Servo object with default values. Should not be used.
    Servo();

    /**
     * @brief Constructs a new Servo object with port.
     * @param port The port of the servo
     */
    Servo(uint8_t port, uint8_t offset);

    /**
     * @brief Constructs a new Servo object with port and max.
     * @param port The port of servo
     * @param max The servo's maximum position, as a decimal from 0 to 1.0
     */
    Servo(uint8_t port, float min, float max, uint8_t offset);

    //! Setup function. Should always run in setup().
    void setupServo();

    /**
     * @brief Sets the servo's target as an 8-bit integer
     * @param target the 8-bit integer that represents the target
     */
    void setTarget(uint8_t target);

    //! Drives servo to target
    void moveToTarget();
};


#endif // SERVO_H
