#include "SECON_PROTOBOARD_V1.h"

ProtoBoard bot;

void setup() {
  // put your setup code here, to run once:
bot.setupBoard();
}
int target = 40;
void loop() {

  //bot.driveMotor(0,100);
  //delay(100);
  int distancetowall = bot.updateDistance()[2];
  bot.driveMotor(0,(distancetowall-target)*10);
  bot.driveMotor(1,(distancetowall-target)*10);

 
}
