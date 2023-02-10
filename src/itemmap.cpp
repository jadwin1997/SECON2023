#include "itemmap.h"

itemMap::itemMap(Bot* bot, const char& type, const std::string& color, const float estAng[]){
    this->type = type;
    this->color = color;

    this->estAng[0] = estAng[0];
    this->estAng[1] = estAng[1];

    float t_pos[2] = {0,0};
    float t_ang = bot->getAngle();
    bot->getPosXY(t_pos);


    estLoc[0] = t_pos[0] + estAng[0]*cos(fmod(t_ang + estAng[1],360));
    estLoc[1] = t_pos[1] + estAng[0]*sin(fmod(t_ang + estAng[1],360));
}

itemMap::itemMap(){
    type = NULL;
    color = NULL;
    estAng[0] = -1;
    estAng[1] = -1;
}

itemMap::itemMap(const std::string& type, const std::string& color, const float estAng[2]){
    itemMap(bot, type, color, estAng);
}

void itemMap::updateAng(const float botPos[2]){
    estAng[0] = hypot(estLoc[0]-botPos[0], estLoc[1]-botPos[1]);

}

void itemMap::updatePos(const float estAng[2]){
    ;
}

void itemMap::initBot(Bot& bot){
    this->bot = &bot;
}
