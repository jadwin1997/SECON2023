

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
float gyro_angle = 0.0;
int bot_size = 12; //9cm is distance from sensor to sensor C
int target_distance = 15;
int* distances;
float elapsedTime, currentTime, previousTime, target_angle,offset = 0;
int pid_old = 0;
int target_speed = 72;

int old_target_speed = target_speed;
int max_speed =80;
int min_speed =10;
float fused_angle = 0.0;
int timer = millis();
void setup() {
  if(millis()-timer<1000){
    target_speed = 100;
  }
  else{
    target_speed = old_target_speed;
  }
  // put your setup code here, to run once:
bot.setupBoard();
  angle_estimate = 0;
  angle_estimate_error = 0;
  process_noise = 0.1;
  measurement_noise = 0.1;
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
int target = 27;
double old_time = 0;
double new_time = 0;

double time_elapsed = 0;

double old_time_2 = millis();
double new_time_2 = 0;

double time_elapsed_2 = 0;

void loop() {
  
  float duration1, distance1, duration2, distance2;
float angle, angle_estimate;
  

    bot.updateGyro();
    bot.updateAccel();
  distances = bot.updateDistance();
  
    delay(50);
    gyro_angle = gyro_angle + (dt)*bot.GyZ;
    old_time_2 = millis();
   int b = distances[3]; //B ultrasonic time value 
   int a = distances[4]; //A ultrasonic time value (using time for more granularity in our measurements)
   int c = distances[2];
   //Serial.print("Ultrasonic 1: ");Serial.print(b);Serial.print(" Ultrasonic 2: ");Serial.print(a);Serial.print(" Front distance sensor: ");Serial.println(c);
   
    
   //calculate angle
   angle = atan2(b-a, bot_size*2.0*29.1);
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
  int front_PID;
  if(c <36){
    front_PID = 87;
    
    target_speed = -15;
    //bot.sendDataToSlave(43);
  }
  else{
    front_PID = 0;
    target_speed = old_target_speed;  
    
  }
  
  

  //erial.println(duration1_estimate);
  bot.kalman_angle = angle_estimate;
  bot.kalman_duration1 = duration1_estimate;
  bot.kalman_duration2 = duration2_estimate;

  
  if(front_PID > 0){
    counter++;
  }
  else{
    counter = 0;
  }






  
  old_time = new_time;
  new_time = millis();
  time_elapsed = new_time - old_time;
  
  
  //gyro_angle = gyro_angle*0.2+angle_estimate*0.5-PID*0.3;
  int left;
  int right;
  int PID = (target_distance-((distances[0]+distances[1])/2));
  if(PID > 10){
    PID = 10;
  }
  if(PID < -10){
    PID = -10;
  }
  fused_angle = 0.6*-PID+0.4*angle_estimate;
  if(front_PID > 0){
    left = 77+target_speed;
    right = -77+target_speed;
  }
  else{
  left = target_speed-fused_angle*6;//target_speed-(angle_estimate*5)+PID;//map(255-gyro_angle*10,-255,255,-target_speed,target_speed)+front_PID;//map(255//(-angle_estimate*3) +PID - front_PID, -255, 255, -target_speed, target_speed);//
  right = target_speed+fused_angle*6;//map(255+gyro_angle*10,-255,255,-target_speed,target_speed)-front_PID;//map(255//(angle_estimate*3) -PID + front_PID, -255, 255, -target_speed, target_speed);//
  }
  
  
  String data = "Angle From Wall: "+String(c);
  Serial.println(gyro_angle);
  
//  if(abs(gyro_angle)>180.0){
//    left = 0;
//    right = 0;
//    bot.driveMotor(0,left);
//    bot.driveMotor(1,right);
//    delay(1000);
//    bot.driveMotor(0,-100);
//    bot.driveMotor(1,-100);
//    delay(3000);
//    left = 0;
//    right = 0;
//    bot.driveMotor(0,left);
//    bot.driveMotor(1,right);
//    delay(1000);
//    gyro_angle = 0;
//    
//  }


    if(abs(gyro_angle)>230.0){
                bot.driveMotor(0,map(target_speed,0,255,0,235));
    bot.driveMotor(1,map(target_speed,0,255,0,235));
    
    delay(1000);
    bot.driveMotor(0,map(0,0,255,0,235));
    bot.driveMotor(1,map(0,0,255,0,235));
    delay(1000);
    }
    else{
          bot.driveMotor(0,map(255,0,255,0,245));
    bot.driveMotor(1,map(255,0,255,0,245));
    }

 
 bot.serialWrite();
  
}
