#include "bot.h"

using namespace std::chrono_literals;

Bot::Bot()
{
    collisionMesh[0] = 0; collisionMesh[1] = 0; collisionMesh[2] = 0;
    pos[0] = 48.0; pos[1] = 7.0;
}

Bot::Bot(const float collisionMesh[3], Sensor sensors[]){
    this->collisionMesh[0] = collisionMesh[0]; this->collisionMesh[1] = collisionMesh[1]; this->collisionMesh[2] = collisionMesh[2];

    this->sensors = sensors;
//    for(unsigned int i=0; i < sizeof(&sensors)/sizeof(Sensor); i++){
//        this->sensors.push_back(sensors[i]);
//    }
}

float Bot::getSpeed(){
    return speed;
}

void Bot::getVelocityXY(float vel[2]){
    vel[0] = velocity[0];
    vel[1] = velocity[1];
}

void Bot::getVelocityAngular(float velAng[2]){
    velAng[0] = std::hypot(velocity[0], velocity[1]);
    velAng[1] = std::atan(velocity[1] / velocity[0]);
}

//bool Bot::collision(const itemMap items[23]){
//    return false; // Temp. placeholder
//}

float Bot::getAngle(){
    return angle;
}

void Bot::getPosXY(float pos[2]){
    pos[0] = this->pos[0];
    pos[1] = this->pos[1];
}

void Bot::setAngle(const float &angle){
    velocity[0] = speed * cos(angle);
    velocity[1] = speed * sin(angle);
}

void Bot::locate(float posVector[3], Sensor sensors[]){
    posVector[0] = pos[0]; posVector[1] = pos[1]; posVector[2] = collisionMesh[2]/2; // Temp. testing method, returns exact position

    // Start of sensor-based location method
    uint16_t t_data = 0;
    for(uint16_t i=0; i < sizeof(&sensors)/sizeof(Sensor); i++){
        if(sensors[i].getType()=="sonar"){
            t_data = sensors[i].sensorRead();
        }else if(sensors[i].getType()=="lidar"){
            t_data = sensors[i].sensorRead();
        }
    }
}

void Bot::p_move(const float &acceleration, const float &speed){ // Moves bot at desired speed and to desired angle (from +x-axis)
    float acceleration_ms = acceleration * (TIME_STEP / 1s);

    // Signed integers of magnitude 1, for simplified if statements and equations
    int sSign = 1;
    if(this->speed > speed){sSign = -1;}

    // Accelerates to desired speed and turns
    if(this->speed != speed){
        if(fabs(this->speed - speed) >= acceleration_ms){
            this->speed += sSign*acceleration_ms; // Accelerates at given accel. value (in/TS)
        }else{
            this->speed = speed; // Throttles down acceleration to reach exact desired speed
        }
    }
}

void Bot::p_turn(const float &turnSpeed, const float &turnAngle){
    float turnSpeed_TS = turnSpeed * (TIME_STEP / 1s);
    int aSign = 1;

    if(remainder((turnAngle - angle + 360), 360) < 180){aSign = -1;}

    if(getAngle() != turnAngle){
        if(fabs(getAngle() - turnAngle) > turnSpeed_TS){
            setAngle(getAngle() + aSign*turnSpeed_TS); // Turns at given turn speed in time-step increments (deg/TS)
        }else{
            setAngle(turnAngle); // Reduces angular velocity to reach exact desired angle
        }
    }
}

void Bot::p_stop(const float &acceleration){
    float acceleration_TS = acceleration * (TIME_STEP / 1s);
    int sSign = 1;
    if(speed > 0){sSign = -1;}
    if(speed!=0){
        if(fabs(speed) >= acceleration_TS){
            speed += sSign*acceleration_TS;
        }else{
            speed = 0;
        }
    }
}

void Bot::p_moveTo(const float &acceleration, const float &turnSpeed, const uint16_t pos[2]){
    // Insert pathfind algorithms
    ;
}

void Bot::move(const float &speed){
    p_move(acceleration, speed);
}

void Bot::turn(const float &turnAngle){
    p_turn(turnSpeed, turnAngle);
}

void Bot::stop(){
    p_stop(acceleration);
}

void Bot::moveTo(const uint16_t pos[2]){
    p_moveTo(acceleration, turnSpeed, pos);
}
