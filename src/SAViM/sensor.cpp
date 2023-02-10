#include "sensor.h"

Sensor::Sensor()
{
    type = "null";
    posOnBot[0] = 0; posOnBot[1] = 0; posOnBot[2] = 0; posOnBot[3] = 0;
    ;
}

Sensor::Sensor(const std::string type, const float posOnBot[4]){
    this->type = type;
    this->posOnBot[0] = posOnBot[0]; this->posOnBot[1] = posOnBot[1]; this->posOnBot[2] = posOnBot[2]; this->posOnBot[3] = posOnBot[3];
    ;
}

std::string Sensor::getType(){
    return type;
}

uint16_t Sensor::sensorRead(){
    return data;
}

void Sensor::sensorWrite(int dataRaw){
    this->dataRaw = dataRaw;
}

//template<typename Data> void Sensor::getData(Data data[], const std::string &type){
//    ;
//}

//void Sensor::updateSensor(Board board[], itemMap items[]){
//    ;
//}

//template<typename Data> void Sensor::getData(Data data[]){
//    if(type=="lidar"){
//        ;
//    }else if(type=="sonar"){
//        ;
//    }else if(type=="camera"){
//        ;
//    }
//}
