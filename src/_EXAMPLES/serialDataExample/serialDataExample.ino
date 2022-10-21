//This code will read a message from the user in the form "x(some number}" and blink the LED with that size delay.

void setup() {
  // put your setup code here, to run once:

//Start serial monitor
Serial.begin(9600);

//Set up LED pin (as you can see, we are using the arduinos built-in LED)
pinMode(LED_BUILTIN,OUTPUT);
}

//Create time_delay variable
int time_delay = 1000;

void loop() {
  // put your main code here, to run repeatedly:
//Write light high  
digitalWrite(LED_BUILTIN,HIGH);
//Delay
delay(time_delay);
//Write light low
digitalWrite(LED_BUILTIN,LOW);
//Delay
delay(time_delay);

//If serial data available, update the time_delay
if(Serial.available()>0){
if(Serial.read()=='x'){
  //wait for x, then save number after it.
  time_delay = Serial.parseInt();
}
}
}


