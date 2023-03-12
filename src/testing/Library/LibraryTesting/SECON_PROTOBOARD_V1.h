



#ifndef robot_lib
#define robot_lib

#include "Arduino.h"
#include <Wire.h>
#include "TFMini.h"
#include <SoftwareSerial.h>
#include <SharpIR.h>

//#include <Wire.h>


class ProtoBoard
{
  
  public:
   ProtoBoard();

    void setupBoard();
    void driveMotor(int motor_pin, int motor_speed);
    int * updateDistance();
    int16_t updateGyro();
    int16_t updateAccel();
    void serialWrite();
    //void sendDataToSlave(int data);
    int serialRead();
    int updateAngle();
    int calibration = 0;
    float variance = 0.0;
    int distance0_readings[10];
    int distance1_readings[10];
    int distance2_readings[10];
    int angle_readings[10];
    void updateVariance(float angle);
    float sum,sum2,count = 0;
    
    
    /**
    * @brief I2C address of the MPU-6050
    **/
    const int MPU_addr=0x68;  //
    int refresh_rate = 20;
    /**
    * @brief Acceleration felt by gyro
    **/
    int16_t GyZ;
  
    float Ax,Ay,Az;
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
    float kalman_angle = 0;
    float kalman_distance1 = 0;
    float kalman_distance2 = 0;
    float kalman_duration1 = 0;
    float kalman_duration2 = 0;
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

    
    int leftpwm = 11; /**< pin for motor driver pwm pin*/
    int rightpwm = 3; /**< pin for motor driver pwm pin*/
    int in1 = 4; /**< motor driver direction pin */
    int in2 = 2; /**< motor driver direction pin */
    int in3 = 8; /**< motor driver direction pin */
    int in4 = 7; /**< motor driver direction pin */

  private:


    
};

#endif
