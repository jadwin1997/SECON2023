#include "SECON_PROTOBOARD_V1.h"
const float dt = 0.05;  // Time step
float angle_estimate,angle_estimate_error,process_noise,measurement_noise,kalman_gain,duration1_estimate,duration1_estimate_error,duration1_process_noise,
duration1_measurement_noise,duration1_kalman_gain,duration2_estimate,duration2_estimate_error,duration2_process_noise,duration2_measurement_noise,duration2_kalman_gain;
int counter = 0;
ProtoBoard bot;
float angle = 0.0;
int bot_size = 9; //9cm is distance from sensor to sensor C
int target_distance = 18;
int* distances;
float elapsedTime, currentTime, previousTime, target_angle,offset = 0;
int pid_old = 0;
int target_speed = 125;
int max_speed = 200;
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
   
  duration1_estimate_error = duration1_estimate_error + duration1_process_noise * dt;
  duration1_kalman_gain = duration1_estimate_error / (duration1_estimate_error + duration1_measurement_noise);
  duration1_estimate = duration1_estimate + duration1_kalman_gain * (b - duration1_estimate);
  duration1_estimate_error = (1 - duration1_kalman_gain) * duration1_estimate_error;

  duration2_estimate_error = duration2_estimate_error + duration2_process_noise * dt;
  duration2_kalman_gain = duration2_estimate_error / (duration2_estimate_error + duration2_measurement_noise);
  duration2_estimate = duration2_estimate + duration2_kalman_gain * (a - duration2_estimate);
  duration2_estimate_error = (1 - duration2_kalman_gain) * duration2_estimate_error;
   
   //Serial.print("Ultrasonic 1: ");Serial.print(b);Serial.print(" Ultrasonic 2: ");Serial.print(a);Serial.print(" Front distance sensor: ");Serial.println(c);
   delay(50);
    
   //calculate angle
   angle = atan2(duration2_estimate - duration1_estimate, 5.0*2.0*29.1);
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


/*
    angle_estimate_error = angle_estimate_error + process_noise * dt;
  kalman_gain = angle_estimate_error / (angle_estimate_error + measurement_noise);
  angle_estimate = angle_estimate + kalman_gain * (angle - angle_estimate);
  angle_estimate_error = (1 - kalman_gain) * angle_estimate_error;
*/
  // Print estimated angle
  int front_PID;
  if(c <35){
    front_PID = 150;
  }
  else{
    front_PID = 0;
  }
  
  int PID = target_distance-distances[0];
  if(PID > 100||PID<-100){
    PID=0;
  }
  //erial.println(duration1_estimate);
  bot.kalman_angle = angle_estimate;
  bot.kalman_duration1 = duration1_estimate;
  bot.kalman_duration2 = duration2_estimate;
  if(abs((bot.Ay+bot.Ax)/2)>0.22){
    counter = 0;
  }
  else{
    counter++;
  }
  if(counter>60){
    target_speed++;
    counter = 0;
  }
  if(target_speed>max_speed){
    target_speed = max_speed;
  }
  int left , right = mapMotorSpeed(angle_estimate, PID, front_PID, target_speed);
  //left= map(115-(-angle_estimate*4)+PID-front_PID, -255, 255, -target_speed, target_speed);
  //int right = map(115-(angle_estimate*4)-PID+front_PID, -255, 255, -target_speed, target_speed);
  bot.driveMotor(0,left);
  bot.driveMotor(1,right); 
 
 bot.serialWrite();
}

int mapMotorSpeed(float angle_estimate, float PID, float front_PID, int target_speed) {
  int left_speed = map(115 + (-angle_estimate * 5) + PID + front_PID, 
                       -max_speed, max_speed, -target_speed, target_speed);
  int right_speed = map(115 - (angle_estimate * 5) - PID + front_PID, 
                        -max_speed, max_speed, -target_speed, target_speed);
  return left_speed, right_speed;
}
