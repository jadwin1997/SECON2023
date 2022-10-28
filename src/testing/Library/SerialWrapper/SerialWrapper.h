#include "Arduino.h"

#ifndef SERIALWRAPPER_H
#define SERIALWRAPPER_H


class SerialWrapper {
    private:
    
    public:
    SerialWrapper();
    bool start();
    void stop();
    void read();
    void write();
    
};

#endif // SERIALWRAPPER_H
