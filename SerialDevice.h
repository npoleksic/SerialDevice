#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H
#include <Arduino.h>

class SerialDevice {
  private:
    Stream &mPort;
    char mChar1;
    int32_t m32Bit;
    char mChar2;
    int16_t m16Bit;

  public:
    SerialDevice(Stream &port);
    
    void writeMessage(char h1, int32_t v1 = 0, char h2 = '_', int16_t v2 = 0);

    char readMessage();
    
    bool awaitMessage(char msg, bool toggle, int pinToToggle = 0);

    bool messageAvailable();

    void flushReceiveBuffer();

    int32_t get32BitValue();

    char getChar2();

    int16_t get16BitValue();
};

#endif