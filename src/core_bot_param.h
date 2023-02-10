/***********************************************************
 *
 * Notes:
 *
 * All measurements are standardized as follows:
 *      Time            -   s
 *      Distance        -   in
 *      Speed           -   in/s
 *      Acceleration    -   in/s^2
 *      Angles          -   deg
 *
 **********************************************************/
#include <chrono>
#include <sensor.h>

#ifndef CORE_BOT_PARAM_H
#define CORE_BOT_PARAM_H

/**
 * @brief botSize
 * Size array: length (front to back), width, height. Used for collision mesh.
 */
inline float botSize[3] = {1,2,3};

/**
 * @brief botAccel
 * Maximum rate of acceleration (in/s^2).
 */
inline float botAccel = 1;

/**
 * @brief botSpeed
 * Top speed (in/s).
 */
inline float botSpeed = 0.5;

/**
 * @brief maxTurn
 * Max turn speed (deg/s).
 */
inline float maxTurn = 60;

/**
 * @brief sensorList
 * All sensors and types on bot.
 */
inline Sensor sensorList[0] = {};
inline int sensorNum; // Assigned later in Bot class via for loop, do not modify

/**
 * @brief startPos
 * Starting position of bot (x,y) (in).
 */
inline float startPos[2] = {48, 6.6875};

/**
 * @brief startAng
 * Starting angle of front of bot from +x-axis (deg).
 */
inline float startAng = 90;

/**
 * @brief TIME_STEP
 * Defined time step for calculation resolution. Default: 1 ms.
 */
std::chrono::duration TIME_STEP = std::chrono::milliseconds(1);

#endif // CORE_BOT_PARAM_H
