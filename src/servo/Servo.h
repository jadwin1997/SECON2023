/**
 * @file Servo.h
 * @brief Controls individual servo
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

    //! Maximum position of servo, 0.0 to 1.0.
    float max;

    //! PWM driver object.
    Adafruit_PWMServoDriver pwm;

    public:
    //! Construct a new Servo object with default values. Should not be used.
    Servo();

    /**
     * @brief Construct a new Servo object with port.
     * @param port Port of servo
     */
    Servo(uint8_t port);

    /**
     * @brief Construct a new Servo object with port and max.
     * @param port Port of servo
     * @param max Maximum position, as a decimal from 0 to 1.0
     */
    Servo(uint8_t port, float max);

    //! Setup function. Should always run in setup().
    void setupServo();

    /**
     * @brief Set the servo's target as an 8-bit integer
     * @param target 8-bit integer target
     */
    void setTarget(uint8_t target);

    //! Drives servo to target
    void moveToTarget();
};


#endif // SERVO_H