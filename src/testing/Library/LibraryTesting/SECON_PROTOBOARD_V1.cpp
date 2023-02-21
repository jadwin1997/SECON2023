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
  
  //delayMicroseconds(2);
  duration1 = pulseIn(9, HIGH);
  
  digitalWrite(6, LOW);
  delayMicroseconds(2);
  digitalWrite(6, HIGH);
  delayMicroseconds(4);
  digitalWrite(6, LOW);
  
  duration2 = pulseIn(5, HIGH);
  

  // Calculating the distance
  distance1 = int((duration1/2) / 29.1); // Speed of sound wave divided by 2 (go and back)
  distance2 = int((duration2/2) / 29.1); // Speed of sound wave divided by 2 (go and back)
  distance3 = SharpIR.distance();

  r[0]=distance1;
  r[1]=distance2;
  r[2]=distance3;
  r[3] = duration1;
  r[4] = duration2;

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
  updateAngle();
}

int16_t ProtoBoard::updateAccel() {
  //Start I2C transmission with accelerometer
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true);  // request a total of 6 registers for the X, Y, and Z acceleration values
  int16_t accelX = (Wire.read() << 8 | Wire.read());
  int16_t accelY = (Wire.read() << 8 | Wire.read());
  int16_t accelZ = (Wire.read() << 8 | Wire.read());

  //convert raw values to acceleration in m/s^2
  float ax = accelX / 16384.0;
  float ay = accelY / 16384.0;
  float az = accelZ / 16384.0;

  //store acceleration values in class variables
  Ax = ax;
  Ay = ay;
  Az = az;

  return Ax;
}
void ProtoBoard::driveMotor(int motor_pin, int motor_speed){
  if(motor_speed < 0){
    motor_speed = -motor_speed;
    if(motor_speed>255){
      motor_speed = 255;
    }
    if(motor_pin == 0){
      digitalWrite(in1,LOW);
      digitalWrite(in2,HIGH);     
      analogWrite(rightpwm,motor_speed);
      }
    if(motor_pin > 0){
      digitalWrite(in3,HIGH);
      digitalWrite(in4,LOW);      
      analogWrite(leftpwm,motor_speed);
      }
  }
  else{
   if(motor_speed>255){
      motor_speed = 255;
    }
  if(motor_pin == 0){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);

    analogWrite(rightpwm,motor_speed);
  }
   if(motor_pin > 0){
    digitalWrite(in3,LOW);
    digitalWrite(in4,HIGH);
    analogWrite(leftpwm,motor_speed);
    
  }
  }
  
}

void ProtoBoard::serialWrite(){
  String data = "F"+String(distance3)+"L"+String(distance1)+"R"+String(distance2)+"LD"+String(kalman_duration1)+"RD"+String(kalman_duration2)+"KA"+String(kalman_angle)+"AC"+String((Ay+Ax)/2);
  Serial.println(data);
}

int ProtoBoard::serialRead(){
  int data = -1;
  if(Serial.available()>0){
    if(Serial.read()=="F"){
      data = Serial.parseInt();
    }
    
  }
  return data;
}

int ProtoBoard::updateAngle(){
  int data = -1;

  return data;
}





