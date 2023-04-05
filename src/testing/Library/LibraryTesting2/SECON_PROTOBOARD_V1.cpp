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
 
  /*
  pinMode(10, OUTPUT); 
  pinMode(9, INPUT); 
  pinMode(6, OUTPUT); 
  pinMode(5, INPUT); 
   */ 


  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT); 
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT); 

  digitalWrite(in2,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in4,HIGH);
  digitalWrite(in3,LOW);

  

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  
  Wire.write(0);     
  Wire.endTransmission(true);
  Serial.begin(9600);

  //button
  double calib= 0;
  for(int x = 0; x< 100; x++){
    updateGyro();
    calib = calib + GyZ;
     
  }
  calibration = calib/100;
}


/**
*@brief updates the ultrasonic distance values, and the SharpIR distance value
*@return returns pointer to array of distances 1-3
**/
int * ProtoBoard::updateDistance(){
  static int r[4];
  /*
  digitalWrite(10, LOW);
  delayMicroseconds(2);
  digitalWrite(10, HIGH);0
  delayMicroseconds(4);
  digitalWrite(10, LOW);
  //duration1 = pulseIn(9, HIGH);
  delayMicroseconds(100);
  
  digitalWrite(6, LOW);
  delayMicroseconds(2);
  digitalWrite(6, HIGH);
  delayMicroseconds(4);
  digitalWrite(6, LOW);
  //duration2 = pulseIn(5, HIGH);
  delayMicroseconds(100);
  */
 int old_distance = distance1;
  // Calculating the distance
  
  distance1 = sonar1.ping_cm()-1;//int((duration1/2) / 29.1); // Speed of sound wave divided by 2 (go and back)
  if(distance1==0){
    distance1 = old_distance;
  }
  old_distance = distance2;
  distance2 = sonar2.ping_cm();//int((duration2/2) / 29.1); // Speed of sound wave divided by 2 (go and back)
    if(distance2==0){
    distance2 = old_distance;
  }
  distance3 = SharpIR.distance();

  duration1 = sonar1.ping_median(1);
  duration2 = sonar2.ping_median(1);
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
  GyZ=(Wire.read()<<8|Wire.read())/131.0 - calibration;  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L) (16 bit number stored in 2 8 bit registers)
  //subtract offset 
  
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
      digitalWrite(in2,LOW);
      digitalWrite(in1,HIGH);     
      analogWrite(leftpwm,motor_speed);
      }
    if(motor_pin > 0){
      digitalWrite(in4,HIGH);
      digitalWrite(in3,LOW);      
      analogWrite(rightpwm,motor_speed);
      }
      
  }
  else{
   if(motor_speed>255){
      motor_speed = 255;
    }
  if(motor_pin == 0){
    digitalWrite(in2,HIGH);
    digitalWrite(in1,LOW);

    analogWrite(leftpwm,motor_speed);
  }
   if(motor_pin > 0){
    digitalWrite(in4,LOW);
    digitalWrite(in3,HIGH);
    analogWrite(rightpwm,motor_speed);
    
  }
  }
  
}

void ProtoBoard::serialWrite(int left, int right){
  String data = " F: "+String(distance3)+" L: "+String(distance1)+" R: "+String(distance2)+" KA: "+String(kalman_angle)+" L motor: "+String(left)+" R motor: "+String(right)+" V: "+String(variance);
  Serial.println(data);
}

int ProtoBoard::serialRead(){
  int data = -1;
  if(Serial.available()>0){
    if(Serial.read()=="X"){
      data = Serial.parseInt();
    }
    
  }
  return data;
}

int ProtoBoard::updateAngle(){
  int data = -1;

  return data;
}

void ProtoBoard::updateVariance(float value) {
  // Add the new value to the sum and sum of squares
  sum += value;
  sum2 += value * value;
  count++;
  
  // Compute the variance when enough values have been added
  if (count >= 7) {
    float mean = sum / count;
    variance = (sum2 - 2 * sum * mean + count * mean * mean) / (count - 1);
    count = 0;
    sum = 0;
    sum2=0;
    //Serial.println(variance);
  } else {
    
    //return 0.0; // Not enough values yet to compute variance
  }
}
