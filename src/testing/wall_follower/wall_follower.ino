  
//Drive Straight w/Gyro (MPU-6050)
#include<Wire.h>
#include <SoftwareSerial.h>
#include "TFMini.h"
#include <SharpIR.h>
#define ir A7
#define model 20150
SharpIR SharpIR(ir, model);

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t GyZ;
float elapsedTime, currentTime, previousTime,angle = 0.0, target_angle,offset = 0;

//target speed to go straight.
int target_speed = 70;

long duration1; // Ping duration for sensor 2
long duration2; // Ping duration for sensor 2
int distance1; // Calculated distance 1
int distance2; // Calculated distance 2

int leftpwm = 3;
int rightpwm = 11;
int in1 = 8;
int in2 = 7;
int in3 = 4;
int in4 = 2;
int avg_distance = 0;
int target = 20;
int frontDistance;

void setup(){
// setup trigger pin 
  pinMode(10, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(9, INPUT); // Sets the echoPin as an INPUT
  pinMode(6, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(5, INPUT); // Sets the echoPin as an INPUT


  //pinMode(leftpwm, OUTPUT); // Sets the trigPin as an OUTPUT
  //pinMode(rightpwm, OUTPUT); // Sets the trigPin as an OUTPUT

  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT); 
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT); 

  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);

  
  //I2C com with gyro
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);

  //button
  pinMode(7,INPUT_PULLUP);

  

  //calibrate offset
  offset = calibrate();
}

//calibrate by taking 1000 measurements and averaging them. Then, we will subtract that value from our measurements later. 
float calibrate(){

  delay(500);
  float GyZ_avg=0;
  for(int x=0;x<1000;x++){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x47);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,2,true);  // request a total of 14 registers
  GyZ=(Wire.read()<<8|Wire.read())/131.0;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  GyZ_avg=GyZ+GyZ_avg;
  }
  GyZ_avg = GyZ_avg/1000.0;
  return GyZ_avg;
}

void loop(){



  digitalWrite(10, LOW);
  //digitalWrite(4, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(10, HIGH);
  //digitalWrite(4, HIGH);
  delayMicroseconds(4);
  digitalWrite(10, LOW);
  //digitalWrite(4, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(9, HIGH);
  //duration2 = pulseIn(5, HIGH);
  delayMicroseconds(10);
  //digitalWrite(2, LOW);
  digitalWrite(6, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  //digitalWrite(2, HIGH);
  digitalWrite(6, HIGH);
  delayMicroseconds(10);
  //digitalWrite(2, LOW);
  digitalWrite(6, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  //duration1 = pulseIn(3, HIGH);
  duration2 = pulseIn(5, HIGH);
  // Calculating the distance
  distance1 = int((duration1/2) / 29.1); // Speed of sound wave divided by 2 (go and back)
  distance2 = int((duration2/2) / 29.1); // Speed of sound wave divided by 2 (go and back)
  //I think this is correct? inverse tan(y/x) x is body_size, to avoid divide by 0 and because angle increases as body size decreases. 
  //estimated_angle = atan2(body_size,(distance1-distance2));
  
  //avg_distance = (distance1 + distance2)/2;
  

  float anglef = asin((float(duration2-duration1)/(29.1*2*11)))*(180/3.14159265);

  int bx = round( angle); // radians to degrees and rounding
  //angle = anglef*1+angle*0;









  //wait for calibrate button
  if(digitalRead(7)==0){
    offset = calibrate();
  }
  
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds

  //Start I2C transmission with gyro
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x47);  // starting with register 0x47 (GYRO_ZOUT)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,2,true);  // request a total of 2 registers
  GyZ=(Wire.read()<<8|Wire.read())/131.0;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L) (16 bit number stored in 2 8 bit registers)
  //subtract offset 
  GyZ=GyZ-offset;
  //convert from rad/s to rad
  if(!isnan(anglef)&&!isnan(angle)){
  
  angle = (angle + GyZ*elapsedTime)*0.8+anglef*0.2;
  }
  
  else{
  angle =(angle + GyZ*elapsedTime*1); //anglef;//(angle + GyZ*elapsedTime)*0.80+anglef*.2;
  }

  if(millis()>70000){
    target = 35;  
  }
  if(millis()>130000){
    target = 60;
  }
  frontDistance = 130 - target;

  if(SharpIR.distance()<frontDistance){
    angle = angle+20;
  }
  angle = angle+0.1*(target-((distance2+distance1)/2));
  int pid = map(angle,-90,90,-255,255);
  if((abs(pid)+50)>255){
    if(pid<0){
     pid = -205; 
    }
    else{
     pid = 205; 
    }
    
  }
  if(millis()<300000){
  if(pid<0){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(in4,LOW);
    digitalWrite(in3,HIGH);
    
    analogWrite(leftpwm,-pid+50);
    analogWrite(rightpwm,50);
  }
  else{
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    digitalWrite(in4,LOW);
    digitalWrite(in3,HIGH);
    analogWrite(rightpwm,pid+50);
    
    analogWrite(leftpwm,50);
  }
  }
  else{
        digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    digitalWrite(in4,LOW);
    digitalWrite(in3,LOW);
  }

  Serial.print("distance = ");Serial.print(SharpIR.distance());Serial.print("  angle = ");Serial.print(angle);Serial.print("  pid = "); Serial.println(pid);

  
}
