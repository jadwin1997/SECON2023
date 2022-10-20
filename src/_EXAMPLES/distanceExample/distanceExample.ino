void setup() {
  // put your setup code here, to run once:
  //Start the serial monitor with a baud rate of 9600
  Serial.begin(9600);

  //Setup the pins. One will be an output, and one will be an input. 
  pinMode(10, OUTPUT); 
  pinMode(9, INPUT); 
}

//create variables to hold the ping duration and the distance
int duration1 = 0;
int distance1 = 0;
void loop() {
  // put your main code here, to run repeatedly:

  //write LOW to pin and wait a little
  digitalWrite(10, LOW);
  delayMicroseconds(2);

  //write HIGH to pin for 4 microseconds
  digitalWrite(10, HIGH);
  delayMicroseconds(4);
  digitalWrite(10, LOW);

  //wait until the ping is heard back and return the time it took
  duration1 = pulseIn(9, HIGH);

  //convert from duration to centimeters
  distance1 = int((duration1/2) / 29.1);

  //print out the distance we calculated
  Serial.print(distance1);
  Serial.println(" CM");
}
