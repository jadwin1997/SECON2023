

#include "SECON_PROTOBOARD_V1.h"
#include <SoftwareSerial.h>
#include "DFRobot_TCS34725.h"

DFRobot_TCS34725 TCS_color = DFRobot_TCS34725(&Wire, TCS34725_ADDRESS,TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

const float dt = 0.05;  // Time step (Because we have a 50 MS delay in our loop for the sonar to update)

// Parameters for wall angle Kalman Filter
float angle_estimate;
float angle_estimate_error;
float process_noise;
float measurement_noise;
float kalman_gain;
//

// Parameters for ultrasonic sensors kalman filter
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
//

//counter for variance stuck detection algorithm
int counter = 0;

//init protoboard class
ProtoBoard bot;

//angle to wall
float angle = 0.0;

//red and green state counters
int red ;
int green ;

//how many times have we depositted counter
int deposit_count;

//init gyro angle
float gyro_angle = 0.0;

//sensor size for trig function later
int bot_size = 12; //12cm is distance from sensor to sensor C

//initial target distance
int target_distance = 12;

//distance sensor array
int* distances;

//times for gyro integration later (we ended up using our dt time step instead)
float elapsedTime, currentTime, previousTime, target_angle,offset = 0;


//target speed when going straight (slow down when adjusting angle)
int target_speed = 120;
//stuck counter previously used to adjust target speed to reduce stalling 
int stuck_counter = 0;

// old_target_speed holds origial target_speed at startup
int old_target_speed = target_speed;


// fused sensor angle between 2 ultrasonics and the front facing LIDAR
float fused_angle = 0.0;

//timer for autostart 
int timer = millis();

//counter for stuck detection
int stuck_loop_counter = 0;

//detect if color sensor is working
int LED_working = 0;

//software serial for communication with arduino mega
SoftwareSerial softSerial = SoftwareSerial(13,12);

//Kp and prev_error for KD algorithm 
float Kp = 0.8;
int prev_error = 0;



void setup() {
  //setup software serial pins
  pinMode(12,OUTPUT);
  pinMode(13,INPUT);

  //start software serial @ 9600 baud rate
  softSerial.begin(9600);
  
  //function for bot initialization (setups up pins and sensors)
  bot.setupBoard();

  //default values for kalman filter
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
  //

  //try connecting to color sensor 10 times, skip if it doesnt work
  for(int y = 0; y<10; y++){
    if(TCS_color.begin()){
      LED_working = 1;
      break;
    }
    
  }
}


//time holds for gyro integration
double old_time = 0;
double new_time = 0;

double time_elapsed = 0;

double time_elapsed_2 = 0;
double old_time_2 = millis();
double new_time_2 = 0;

//holds color sensor timer
int LED_timer = 0;

//distance difference for KD algorithm
int distance_diff_error;


void loop() {
  
  distance_diff_error = 0;
  deposit_count = 0;
  red = 0;
  green = 0;
  int red_color_sensing = 0;
  int time_now_LED = millis();

  //wait for LED autostart. Expires after 10 seconds
  while(1==1 && LED_working == 1 ){
    //Get color sensor value and convert red value to hexadecimal
        uint16_t red, green, blue, clear;
    TCS_color.getRGBC(&red, &green, &blue, &clear);
    TCS_color.lock();
    float r = red;
    r /= clear;
    r *= 256;
    Serial.print((int)r, HEX);
    Serial.println();

      //Start program if LED is detected or time expires
        if(r > 0x80){
      Serial.print("Starting...");
      break;
      }
      LED_timer = millis() - time_now_LED;
      if(LED_timer>10000){
        break;
      }
    
  }

  //main loop for bot.
  while(1==1){
    
  
  float duration1, distance1, duration2, distance2;
  float angle, angle_estimate;
  
    //update sensors
    bot.updateGyro();
    bot.updateAccel();
    //store distances in distances array
  distances = bot.updateDistance();
    //delay 50 ms for sonars to update via non-blocking interrupts
    delay(50);

    //integrate gyro_angle
    gyro_angle = gyro_angle + (dt)*bot.GyZ;
    old_time_2 = millis();

   //store distances sepperately. Should probably just use the array, but it was easier for doing the math
   int b = distances[3]; //B ultrasonic time value 
   int a = distances[4]; //A ultrasonic time value (using time for more granularity in our measurements)
   int c = distances[2];
   //Serial.print("Ultrasonic 1: ");Serial.print(b);Serial.print(" Ultrasonic 2: ");Serial.print(a);Serial.print(" Front distance sensor: ");Serial.println(c);
   
    
   //calculate angle to wall via distance sensors
   angle = atan2(b-a-distance_diff_error, bot_size*2.0*29.1);
angle = angle * 180 / PI;


  // Apply Kalman filter to estimate angle to wall via distance sensors
  angle_estimate_error = angle_estimate_error + process_noise * dt;
  kalman_gain = angle_estimate_error / (angle_estimate_error + measurement_noise);
  angle_estimate = angle_estimate + kalman_gain * (angle - angle_estimate);
  angle_estimate_error = (1 - kalman_gain) * angle_estimate_error;

  //just now noticing I did the kalman filter on the ultrasonic distances, but didnt actually use them LMAO.
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
  
  int front_PID;

  //check if front sensor is less than threshold
  if(c <39){
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

  //if front_PID, add to counter. this is for hysteresis effect (lag), so the bot doesnt turn away instantly
  if(front_PID > 0){
    counter++;
  }
  else{
    counter = 0;
  }






  //update timers for gyro integration
  old_time = new_time;
  new_time = millis();
  time_elapsed = new_time - old_time;
  
  
  
  //PD algorithm 
  int error = (target_distance-(distances[1]));
  int delta_error = error - prev_error;
  int PD = Kp*error+(1-Kp)*delta_error;
  
  //Start adding to fused angle if front wall is detected
  if(front_PID>0){
  fused_angle = fused_angle-3;
  //fused_angle = (-PD*0.9+bot.kalman_angle*0.1)-10;

  //constrain so bot doesnt just spin in circles really fast.
  fused_angle = constrain(fused_angle,-45,45);
  }
  else{

  //complementary filter between the PD algorithm and ultrasonic angle
  fused_angle = -PD*0.9+bot.kalman_angle*0.1;
  fused_angle = constrain(fused_angle,-38,38);
  }

  //motor speed variable. maps angle to target_speed, slowing down when angle is too high. This reduces oscillations and allows the bot to smoothly
  //reset its angle to the wall. 
  int left = map(abs(fused_angle),0,45,target_speed,-15) - fused_angle*3;
  int right = map(abs(fused_angle),0,45,target_speed,-15) + fused_angle *3;

  //constrain to -255->255
  left = constrain(left,-255,255);
  right = constrain(right, -255,255);
  

  
  String data = "Angle From Wall: "+String(c);

    //update variance for stuck detection
    bot.updateVariance((distances[0]+distances[1])/2);

    //if variance is too low, we must be stuck. 
    if(bot.variance<12.00){
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

      //"kick" bot forward for 50 ms. This jerks the bot forward and helps it eventually fix itself if in a local minima/maxima (aka, stuck)
      bot.driveMotor(0,225);
      bot.driveMotor(1,225);
      delay(50);
      bot.driveMotor(0,0);
      bot.driveMotor(1,0);

      //originally used to increase target speed
      if(stuck_loop_counter > 300){
        //old_target_speed = old_target_speed+2;
        stuck_loop_counter = 0;
      }
    }

      //softSerial.print(0);

      //send values to motors.
          bot.driveMotor(0,left);
    bot.driveMotor(1,right);


 // write to serial monitor
 bot.serialWrite(left, right,gyro_angle);

 //check if at red corner using the gyro angle. Hardcoded to back into wall to align itself and drop chips
 if(gyro_angle < -47 && red == 0){
  red = 1;
  bot.driveMotor(0,0);
  bot.driveMotor(1,0);
  delay(1000);
  bot.driveMotor(0,-130);
  bot.driveMotor(1,-140);
  delay(400);
  //drop red chip command
  softSerial.print(2);
  delay(900);
  bot.driveMotor(0,0);
  bot.driveMotor(1,0);
  
  delay(1000);
  red = 1;
 }

 //check if at green corner and drop chips.
 if(gyro_angle <-89 && green == 0){
  green = 1;
  bot.driveMotor(0,0);
  bot.driveMotor(1,0);
  delay(1000);
  bot.driveMotor(0,-110);
  bot.driveMotor(1,-130);
  delay(1000);
  //drop green chip command
  softSerial.print(3);
  delay(1000);

  //reset gyro_angle because we are probably aligned with the left wall
  gyro_angle = -180.0;
  green = 1;
 }

 //check if at recycling. Turn back into recycling and deposit items. Turn a little and drop some more to sweep items into corner
 if(gyro_angle < -275){
  deposit_count++;
  gyro_angle = 360-290;
  gyro_angle = gyro_angle - 71;
  
      bot.driveMotor(0,0);
   bot.driveMotor(1,0);
   delay(1000);

   //initial turn
   bot.driveMotor(0,-128);
   bot.driveMotor(1,128);
   delay(1800);
   bot.driveMotor(0,0);
   bot.driveMotor(1,0);
   
   delay(1000);

   bot.driveMotor(0,100);
   bot.driveMotor(1,100);
   delay(1200);
   
   //drop items
   softSerial.print(1);
   bot.driveMotor(0,0);
   bot.driveMotor(1,0);
   delay(5000);
   //turn into corner more
    bot.driveMotor(0,-150);
   bot.driveMotor(1,-50);
   delay(1100); 
        bot.driveMotor(0,100);
   bot.driveMotor(1,100);
   delay(1300);
      bot.driveMotor(0,0);
   bot.driveMotor(1,0);
   target_distance = target_distance + 15;
   delay(5000);

   //stop after two loops of the field.
   if(deposit_count >=2){
    while(1){
         softSerial.print(1);
         bot.driveMotor(0,0);
         bot.driveMotor(1,0);
         delay(1000);
    }
   }
 }
 else{
  //intake items if not at recycling
  softSerial.print(0);
 }
  
  /*
  if(gyro_angle <-315){
    if(angle_estimate == 0.00){
      gyro_angle = 0.0;
    }
    
  }
  else if(gyro_angle <-225){
    if(angle_estimate == 0.00){
      gyro_angle = -270.0;
    }
  }
  else if(gyro_angle <-135){
    if(angle_estimate == 0.00){
      gyro_angle = -180.0;
    }
  }
  else if(gyro_angle <-45){
    if(angle_estimate == 0.00){
      gyro_angle = -90.0;
    }
  }
  
  */
  
  
  
  
  
  
  
  
  
  
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
