#include "Arduino.h"

#ifndef SERIALWRAPPER_H
#define SERIALWRAPPER_H


class SerialWrapper {
    private:
    
    public:
    SerialWrapper();
    bool start();
    void stop();
    int read();
    void write(int x);
};

#endif // SERIALWRAPPER_H
