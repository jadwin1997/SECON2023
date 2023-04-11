
#include "SECON_PROTOBOARD_V1.h"
#include <SoftwareSerial.h>
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
int red = 0;
int green = 0;
float gyro_angle = 0.0;
int bot_size = 12; //9cm is distance from sensor to sensor C
int target_distance = 13;
int* distances;

float elapsedTime, currentTime, previousTime, target_angle,offset = 0;
int pid_old = 0;
int target_speed = 80;
int stuck_counter = 0;
int old_target_speed = target_speed;
int max_speed =110;
int min_speed =10;
float fused_angle = 0.0;
int timer = millis();
int stuck_loop_counter = 0;
SoftwareSerial softSerial = SoftwareSerial(13,12);
float Kp = 0.8;
int prev_error = 0;
void setup() {
  pinMode(12,OUTPUT);
  pinMode(13,INPUT);
  softSerial.begin(9600);
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
int distance_diff_error;
void loop() {
  int left;
  int right;
  distance_diff_error = 0;//calibrateDistance();
  delay(2000);
  while(1==1){
    
  
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
   angle = atan2(b-a-distance_diff_error, bot_size*2.0*29.1);
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
  if(c <35){
    front_PID = c;
    //softSerial.print(1);
    
    //bot.sendDataToSlave(43);
  }
  else{
    //softSerial.print(0);
    front_PID = 0;
    //target_speed = old_target_speed;  
    
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

  int error = (target_distance-(distances[1]));

  int delta_error = error - prev_error;
  int PD = Kp*error+(1-Kp)*delta_error;
  
  //fused_angle = -PD*0.9+bot.kalman_angle*0.1;
  //fused_angle = constrain(fused_angle,-90,90);
  //eft = map(fused_angle,0,90,target_speed,0);

  if(front_PID>0){
  fused_angle = fused_angle-2;
  //fused_angle = (-PD*0.9+bot.kalman_angle*0.1)-10;
  fused_angle = constrain(fused_angle,-45,45);
  }
  else{
  fused_angle = -PD*0.9+bot.kalman_angle*0.1;
  fused_angle = constrain(fused_angle,-38,38);
  }
  left = map(abs(fused_angle),0,45,target_speed,-15) - fused_angle*3;
  right = map(abs(fused_angle),0,45,target_speed,-15) + fused_angle *3;
  left = constrain(left,-255,255);
  right = constrain(right, -255,255);
  

  
  String data = "Angle From Wall: "+String(c);


    bot.updateVariance((distances[0]+distances[1])/2);
    if(bot.variance<10.00){
      stuck_counter++;
    }
    else{
      stuck_counter = 0;
      target_speed = old_target_speed;
    }
    if(stuck_counter > 15){
      target_speed = target_speed + 4;
      stuck_loop_counter++;
      stuck_counter = 0;
      bot.driveMotor(0,250);
      bot.driveMotor(1,250);
      delay(50);
      bot.driveMotor(0,0);
      bot.driveMotor(1,0);
      if(stuck_loop_counter > 300){
        //old_target_speed = old_target_speed+2;
        stuck_loop_counter = 0;
      }
    }

      //softSerial.print(0);
          bot.driveMotor(0,left);
    bot.driveMotor(1,right);


 
 bot.serialWrite(left, right,gyro_angle);
 if(gyro_angle < -35 && red == 0){
  bot.driveMotor(0,0);
  bot.driveMotor(1,0);
  delay(1000);
  bot.driveMotor(0,-100);
  bot.driveMotor(1,-100);
  delay(1000);
  softSerial.print(2);
  delay(1000);
  red = 1;
 }
 if(gyro_angle <-80 && green == 0){
  bot.driveMotor(0,0);
  bot.driveMotor(1,0);
  delay(1000);
  bot.driveMotor(0,-100);
  bot.driveMotor(1,-100);
  delay(1000);
  softSerial.print(3);
  delay(1000);
  green = 1;
  
 }
 if(gyro_angle < -210){
  gyro_angle = 360-210;
   
      bot.driveMotor(0,0);
   bot.driveMotor(1,0);
   delay(1000);
   softSerial.print(1);
   delay(2000);
   bot.driveMotor(0,0);
   bot.driveMotor(1,0);
   delay(2000);
  
    bot.driveMotor(0,-120);
   bot.driveMotor(1,-120);
   delay(1500); 
   target_distance = target_distance + 3;
 }
 else{
  softSerial.print(0);
 }
  }
}

int calibrateDistance(){
  int sum = 0;
  for(int x = 0; x < 5; x++){
    delay(40);
    distances = bot.updateDistance();
    sum = sum + (distances[3]-distances[4]);
  }
  return sum/5;
}

