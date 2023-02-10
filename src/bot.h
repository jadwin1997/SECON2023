#include <core_bot_param.h>
#include <board.h>
#include <sensor.h>

#include <vector>
#include <chrono>
#include <thread>

#ifndef BOT_H
#define BOT_H


class Bot{
private:

    /**
     * @brief Position vector
     * Position coordinates in (x,y) format. Initializes at (48",6 11/16"), the center of the start area.
     */
    float pos[2] = {startPos[0],startPos[1]};

    /**
     * @brief Angle of front of bot from +x-axis
     * Initializes at start angle from Core Bot Parameters.
     */
    float angle = startAng;

    /**
     * @brief Maximum bot size
     * Constant at 12" x 12" x 12", as per competition regulations.
     */
    float maxSize[3] = {12, 12, 12};

    /**
     * @brief Maximum speed in in/s
     */
    float maxSpeed = botSpeed;

    /**
     * @brief Maximum acceleration in in/s^2
     */
    float acceleration = botAccel;

    /**
     * @brief Maximum turn speed in deg/s
     */
    float turnSpeed = maxTurn;

    /**
     * @brief Collision mesh
     * Model for collision avoidance. Actual rigid dimensions of bot.
     */
    float collisionMesh[3] = {botSize[0],botSize[1],botSize[2]};

    /**
     * @brief Velocity vector
     * Vector value in [x,y] format.
     */
    float velocity[2];

    /**
     * @brief Instantaneous speed scalar
     * Speed of the bot, regardless of direction.
     */
    float speed;

//    /**
//     * @brief Vector of sensors
//     * Size varies based on user-added sensor objects.
//     */
//    std::vector<Sensor> sensorsA;

    /**
     * @brief sensors
     * Points to sensor array.
     */
    Sensor* sensors;


    /**********************************************************
    * Private calls with acceleration and turn speed constants
    **********************************************************/

    /**
     * @brief Private move
     * Called by public {@link Bot#move}.
     * @param acceleration
     * @param turnSpeed
     * @param speed
     * @param turnAngle
     */
    void p_move(const float &acceleration, const float &speed); // Adjusts bot speed

    /**
     * @brief p_turn
     * Called by public {@link Bot#turn}.
     * @param turnSpeed
     * @param turnAngle
     */
    void p_turn(const float &turnSpeed, const float &turnAngle); // Adjusts bot direction

    /**
     * @brief p_stop
     * Called by public {@link Bot#stop}.
     * @param acceleration
     */
    void p_stop(const float &acceleration); // Reduces bot speed to zero

    /**
     * @brief p_moveTo
     * Called by public {@link Bot#moveTo}. Use pathfinding alg. to reach destination.
     * @param acceleration
     * @param speed
     * @param pos
     */
    void p_moveTo(const float &acceleration, const float &turnSpeed, const uint16_t pos[2]); // Moves to given coordinate

public:

    /*****************************************************
     * Constructors
     ****************************************************/

    /**
     * @brief Bot default
     * Default constructor.
     */
    Bot();

    /**
     * @brief Bot (specified)
     * @param size
     * @param sensors
     */
    Bot(const float size[3], Sensor sensors[]); // Constructor for bot with input size for collision mesh and sensors

    /******************************************************
     * Getters
     *****************************************************/

    /**
     * @brief getSpeed
     * @return speed
     */
    float getSpeed(); // Returns speed scalar (in/s)

    /**
     * @brief getVelocityXY
     * @param vel
     */
    void getVelocityXY(float vel[2]); // Returns velocity (x,y vector format) to param. array (in/s)

    /**
     * @brief getVelocityAngular
     * @param velAng
     */
    void getVelocityAngular(float velAng[2]); // Returns velocity (speed-angle format) to param. array (in/s, deg. from +x-axis)

    /**
     * @brief getAngle
     * @return angle
     */
    float getAngle(); // Returns current bot direction (deg. from +x-axis)

    /**
     * @brief getPosXY
     * @param pos
     * Returns position of bot (x,y) (in).
     */
    void getPosXY(float pos[2]);


    /*********************************************************
     * Setters
     ********************************************************/

    /**
     * @brief setAngle
     * @param angle
     */
    void setAngle(const float &angle); // Sets current angle & adjusts member array "velocity"

//    /**
//     * @brief collision
//     * @param items
//     * @return
//     */
//    bool collision(const itemMap items[23]); // Checks for collision with items and/or board walls

    /*************************************************
     * Sensor read/write functions
     ************************************************/

    /**
     * @brief locate
     * @param posVector
     * @param sensors
     */
    void locate(float posVector[3], Sensor sensors[]); // Uses relevant mounted sensors to find current position

    /*************************************************
     * Motor write functions
     ************************************************/

    /**
     * @brief move
     * Calls private {@link Bot#p_move}.
     * @param speed
     */
    void move(const float &speed); // Calls "move" private member method

    /**
     * @brief turn
     * Calls private {@link p_turn}.
     * @param turnAngle
     */
    void turn(const float &turnAngle); // Calls "turn" private member method

    /**
     * @brief stop
     */
    void stop(); // Calls "stop" private member method

    /**
     * @brief moveTo
     * Calls private {@link p_moveTo}. Implementation of path-finding algorithm and sensor fusion.
     * @param speed
     * @param pos
     */
    void moveTo(const uint16_t pos[2]); // Calls "moveTo" private member method

};

#endif // BOT_H
