

#include "SECON_PROTOBOARD_V1.h"
const float dt = 0.05;  // Time step
float angle_estimate;
float angle_estimate_error;
float process_noise;
float measurement_noise;
float kalman_gain;

float duration1_estimate;
float duration1_estimate_error;
float duration1_process_noise;
float duration1_measurement_noise;
float duration1_kalman_gain;

float duration2_estimate;
float duration2_estimate_error;
float duration2_process_noise;
float duration2_measurement_noise;
float duration2_kalman_gain;
int counter = 0;
ProtoBoard bot;
float angle = 0.0;
int bot_size = 9; //9cm is distance from sensor to sensor C
int target_distance = 18;
int* distances;
float elapsedTime, currentTime, previousTime, target_angle,offset = 0;
int pid_old = 0;
int target_speed = 125;
int max_speed = 142;
int kP_angle = 4;
int kP_distance = 4;
int kP_front;
void setup() {
  // put your setup code here, to run once:
bot.setupBoard();
  angle_estimate = 0;
  angle_estimate_error = 0;
  process_noise = 0.05;
  measurement_noise = 0.05;
  kalman_gain = 0;

  duration1_estimate = 0;
  duration1_estimate_error = 0;
  duration1_process_noise = 0.05;
  duration1_measurement_noise = 0.05;
  duration1_kalman_gain = 0;

  duration2_estimate = 0;
  duration2_estimate_error = 0;
  duration2_process_noise = 0.05;
  duration2_measurement_noise = 0.05;
  duration2_kalman_gain = 0;
}
int target = 20;
void loop() {
  float duration1, distance1, duration2, distance2;
float angle, angle_estimate;

    bot.updateGyro();
    bot.updateAccel();
  distances = bot.updateDistance();
   int b = distances[3]; //B ultrasonic time value 
   int a = distances[4]; //A ultrasonic time value (using time for more granularity in our measurements)
   int c = distances[2];
   //Serial.print("Ultrasonic 1: ");Serial.print(b);Serial.print(" Ultrasonic 2: ");Serial.print(a);Serial.print(" Front distance sensor: ");Serial.println(c);
   delay(50);
    
   //calculate angle
   angle = atan2(a - b, 5.0*2.0*29.1);
angle = angle * 180 / PI;
// Apply Kalman filter to estimate angle

// Print estimated angle
//Serial.println(angle);

    //TODO: ADD KALMAN FILTER TO LIBRARY, CREATE EXAMPLE CODE, AND UPLOAD TO GITHUB
   // Estimate the angle using the Kalman filter
  angle_estimate_error = angle_estimate_error + process_noise * dt;
  kalman_gain = angle_estimate_error / (angle_estimate_error + measurement_noise);
  angle_estimate = angle_estimate + kalman_gain * (angle - angle_estimate);
  angle_estimate_error = (1 - kalman_gain) * angle_estimate_error;

  duration1_estimate_error = duration1_estimate_error + duration1_process_noise * dt;
  duration1_kalman_gain = duration1_estimate_error / (duration1_estimate_error + duration1_measurement_noise);
  duration1_estimate = duration1_estimate + duration1_kalman_gain * (b - duration1_estimate);
  duration1_estimate_error = (1 - duration1_kalman_gain) * duration1_estimate_error;

  duration2_estimate_error = duration2_estimate_error + duration2_process_noise * dt;
  duration2_kalman_gain = duration2_estimate_error / (duration2_estimate_error + duration2_measurement_noise);
  duration2_estimate = duration2_estimate + duration2_kalman_gain * (a - duration2_estimate);
  duration2_estimate_error = (1 - duration2_kalman_gain) * duration2_estimate_error;
/*
    angle_estimate_error = angle_estimate_error + process_noise * dt;
  kalman_gain = angle_estimate_error / (angle_estimate_error + measurement_noise);
  angle_estimate = angle_estimate + kalman_gain * (angle - angle_estimate);
  angle_estimate_error = (1 - kalman_gain) * angle_estimate_error;
*/
  // Print estimated angle
  int distance_error = target_distance-(distances[0]+distances[1])/2;
  int PID_angle = kP_angle * angle_estimate;
  int PID_distance = kP_distance * distance_error;

  if(c < 30){
    kP_front = 4;
  }
  else{
    kP_front = 0;
  }
  int PID_front_distance = kP_front * (30 - c);
  bot.kalman_angle = angle_estimate;
  bot.kalman_duration1 = duration1_estimate;
  bot.kalman_duration2 = duration2_estimate;
  
  int left = target_speed + (PID_angle*.33+PID_distance*.33+PID_front_distance*.33);
  int right = target_speed - (PID_angle*.33+PID_distance*.33+PID_front_distance*.33);
  left = min(left, max_speed);
  right = min(right, max_speed);
  bot.driveMotor(0,left);
  bot.driveMotor(1,right); 
 
 bot.serialWrite();
}

void setupMotors(){
  
}
