#include "Servo.h"

#define MIN_PULSE_WIDTH       800
#define MAX_PULSE_WIDTH       2200
#define FREQUENCY             50


Servo::Servo() : pwm()
{
    /*
    this shouldn't ever be called; it is only defined because the
    compiler gets upset if there is no default constructor.

    it's probably possible to get around this? todo.
    */
    port = 255;
    target = 0;
    min = 1;
    max = 1;

    offset = 0;
}


Servo::Servo(uint8_t port, uint8_t offset) : pwm()
{
    this->port = port;
    target = 0;
    min = 1;
    max = 1;

    this->offset = offset;
}

Servo::Servo(uint8_t port, float min, float max, uint8_t offset) : pwm()
{
    this->port = port;
    target = 0;
    this->min = min;
    this->max = max;

    this->offset = offset;
}


void Servo::setupServo()
{
    // start up pwm and set frequency
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);
}


void Servo::setTarget(uint8_t target)
{
    // set target by multiplying it with max
    // there's probably a more efficient way to do this
    // this->target = uint8_t(float(target) * max);
    if(target + offset >= target)
    {
        target = 0;
    }
    else
    {
        this->target = target + offset;
    }
}


void Servo::moveToTarget()
{
    // does some fancy math magic to drive servo
    // todo: see if int is overkill?
    int pulse_wide = map(target, 0, 255,
                        MIN_PULSE_WIDTH * min,
                        MAX_PULSE_WIDTH * max);
    int pulse_width = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);

    // drive servo
    pwm.setPWM(port, 0, pulse_width);
}
