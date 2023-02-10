#include <board.h>

#ifndef SENSOR_H
#define SENSOR_H

/**
 * @brief The Sensor class
 * Base class for sensor data.
 */
class Sensor
{
protected:
    /**
     * @brief dataRaw
     * Serial data received from sensor via Arduino serial port.
     */
    uint32_t dataRaw;
    /**
     * @brief data
     * Data value converted from I2C raw data input.
     */
    uint16_t data;
    /**
     * @brief fov
     * Angular field of view of the sensor.
     */
    float fov,
    /**
     * @brief posOnBot
     * Coordinates of sensor relative to center of the bot (x, y, z, angle c.c.w. from front).
     */
    posOnBot[4],
    /**
     * @brief range
     * Reliable range of the sensor (radius, min. angle: the furthest c.w., max. angle: the furthest c.c.w.).
     */
    range[3];
    /**
     * @brief type
     * Type name of the sensor (e.g. lidar, sonar, camera, etc.).
     */
    std::string type;

public:
    /**
     * @brief Sensor
     * Default constructor.
     */
    Sensor();
    /**
     * @brief Sensor
     * Standard constructor.
     * @param type      Type name of the sensor (e.g. lidar, sonar, camera, etc.).
     * @param posOnBot  Coordinates of sensor relative to center of the bot (x, y, z, angle c.c.w. from front).
     */
    Sensor(const std::string type, const float posOnBot[4]);

    /**
     * @brief getType
     * @return Type of sensor.
     */
    std::string getType();

    //void updateSensor(Board board[14], itemMap items[]);

    /**
     * @brief sensorRead
     * Returns processed sensor data in unsigned 16-bit integer.
     * @return Sensor data value.
     */
    uint16_t sensorRead();

    void sensorWrite(int dataRaw);

};

#endif // SENSOR_H
