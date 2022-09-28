#include "SECON_PROTOBOARD_V1.h"

ProtoBoard bot;
void setup() {
  // put your setup code here, to run once:
bot.setupBoard();
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(bot.updateDistance()[2]);
}
