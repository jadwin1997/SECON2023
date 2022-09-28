#include "Arduino.h"
#include "SECON_PROTOBOARD_V1.h"
#define ir A7
#define model 20150
SharpIR SharpIR( ir,model);



ProtoBoard::ProtoBoard(){
  
  
  

}

/**
*@brief sets up the arduino nanos pins and wakes up MPU. Must be ran before board can be used. 
**/
void ProtoBoard::setupBoard(){
 

  pinMode(10, OUTPUT); 
  pinMode(9, INPUT); 
  pinMode(6, OUTPUT); 
  pinMode(5, INPUT); 



  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT); 
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT); 

  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);

  

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  
  Wire.write(0);     
  Wire.endTransmission(true);
  Serial.begin(9600);

  //button
  pinMode(7,INPUT_PULLUP);

}


/**
*@brief updates the ultrasonic distance values, and the SharpIR distance value
*@return returns pointer to array of distances 1-3
**/
int * ProtoBoard::updateDistance(){
  static int r[4];
  digitalWrite(10, LOW);
  delayMicroseconds(2);
  digitalWrite(10, HIGH);
  delayMicroseconds(4);
  digitalWrite(10, LOW);
  duration1 = pulseIn(9, HIGH);
  
  delayMicroseconds(10);

  digitalWrite(6, LOW);
  delayMicroseconds(2);
  digitalWrite(6, HIGH);
  delayMicroseconds(10);
  digitalWrite(6, LOW);
  duration2 = pulseIn(5, HIGH);


  // Calculating the distance
  distance1 = int((duration1/2) / 29.1); // Speed of sound wave divided by 2 (go and back)
  distance2 = int((duration2/2) / 29.1); // Speed of sound wave divided by 2 (go and back)
  distance3 = SharpIR.distance();

  r[0]=distance1;
  r[1]=distance2;
  r[2]=distance3;

  return r;
}


/**
*@brief updates the current gyro reading. (NOTE: this only updates rotation acceleration on the Z-axis)
*@return int16_t returns acceration felt by gyro 
**/
int16_t ProtoBoard::updateGyro(){
  //Start I2C transmission with gyro
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x47);  // starting with register 0x47 (GYRO_ZOUT)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,2,true);  // request a total of 2 registers
  GyZ=(Wire.read()<<8|Wire.read())/131.0;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L) (16 bit number stored in 2 8 bit registers)
  //subtract offset 
  GyZ=GyZ-offset;
}





