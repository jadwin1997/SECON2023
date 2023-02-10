#include "lidar.h"

LIDAR::LIDAR(const float posOnBot[], const float &fov)
{
    this->posOnBot[0] = posOnBot[0]; this->posOnBot[1] = posOnBot[1]; this->posOnBot[2] = posOnBot[2]; this->posOnBot[3] = posOnBot[3];
    ;
}

/*void Sensor::updateSensor(Board board[14], Item items[23], Bot bot){
    ;
}*/

float LIDAR::getData(){
    return data;
}
