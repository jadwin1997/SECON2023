#include <sensor.h>
#include <bot.h>

#ifndef LIDAR_H
#define LIDAR_H

class LIDAR: protected Sensor
{
private:
    float rangeVar, data; // Randomization range of sensor data; data value at inst.

    float getData();
public:
    LIDAR(const float posOnBot[4], const float &fov);
};

#endif // LIDAR_H
