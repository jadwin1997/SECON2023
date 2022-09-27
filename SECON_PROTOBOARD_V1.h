



#ifndef robot_lib
#define robot_lib

#include "Arduino.h"
#include <Wire.h>
#include "TFMini.h"
#include <SoftwareSerial.h>
#include <SharpIR.h>



class ProtoBoard
{
  
  public:
   ProtoBoard();

    void setupBoard();
    int * updateDistance();
    int16_t updateGyro();
    /**
    * @brief I2C address of the MPU-6050
    **/
    const int MPU_addr=0x68;  //
    /**
    * @brief Acceleration felt by gyro
    **/
    int16_t GyZ;


    float elapsedTime, currentTime, previousTime,angle = 0.0, target_angle,offset = 0;

    //target speed to go straight.

    int target_speed = 70;

     /**
    * @brief time for ping to comeback to ultrasonic sensor 1 (more accurate)
    **/
    long duration1; // Ping duration for sensor 2

    /**
    * @brief time for ping to comeback to ultrasonic sensor 2 (more accurate)
    **/
    long duration2; // Ping duration for sensor 2

    /**
    * @brief Ultrasonic Distance Sensor 1
    **/
    int distance1; // Calculated distance 1

    /**
    * @brief Ultrasonic Distance Sensor 2
    **/
    int distance2; // Calculated distance 2

    /**
    * @brief IR distance sensor
    **/
    int distance3; // Calculated distance 3

    
    int leftpwm = 3; /**< pin for motor driver pwm pin*/
    int rightpwm = 11; /**< pin for motor driver pwm pin*/
    int in1 = 8; /**< motor driver direction pin */
    int in2 = 7; /**< motor driver direction pin */
    int in3 = 4; /**< motor driver direction pin */
    int in4 = 2; /**< motor driver direction pin */

  private:


    
};

#endif
