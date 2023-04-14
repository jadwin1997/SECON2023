//#include <DFRobot_TCS34725.h>

/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <FastLED.h>


// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 
Servo myservo;  // create servo object to control a servo
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);
SoftwareSerial softSerial = SoftwareSerial(3,2);

//DFRobot_TCS34725 tcs = DFRobot_TCS34725(&Wire, TCS34725_ADDRESS,TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


//LED stuff 

#define DATA_PIN    2
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB
#define NUM_LEDS    13
CRGB leds[NUM_LEDS];

int loops = 0;
int r = 0;
int g = 0;
int b = 0;
int counter = 0;
int step1 = 1;

void setup() {
  myservo.attach(10);
  Serial.begin(9600);
  
  pinMode(2,OUTPUT);
  //pinMode(8,OUTPUT);
 
  softSerial.begin(9600);           // set up Serial library at 9600 bps
  //Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor->setSpeed(150);
  myMotor->run(FORWARD);
  // turn on motor
  myMotor->run(RELEASE);
  myservo.write(90);
  //delay(1000);

    // LED initialization
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(255);


}

void loop() {




  // Rainbow wave effect
  for (int i = 0; i < NUM_LEDS; i++) {
    int hue = counter + i * 20;
    leds[i] = CHSV(hue % 255, 255, 255);
  }
  
  counter += step1;
  if (counter >= 255) {
    step1 = -1;
  }
  if (counter <= 0) {
    step1 = 1;
  }









  //Serial.print("tick");

  //myMotor->run(BACKWARD);
  
    myMotor->setSpeed(255);  
  delay(10);
  FastLED.show();
  delay(10);
  if(Serial.available()>0){
    int data = int(Serial.read());
    Serial.println(String(data));
    if(data == 48){
      myMotor->run(BACKWARD);
    }
    if(data == 49){
      myMotor->run(FORWARD);
    }
    if(data == 50){
      myservo.write(150);
    }
    if(data == 51){
      myservo.write(30);
    }
  }



}
