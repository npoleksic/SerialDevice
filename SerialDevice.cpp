#include "SerialDevice.h"
#include <iostream>
using namespace std;

SerialDevice::SerialDevice(Stream &port) 
  : mPort(port) {
  mChar1 = '_';
  m32Bit = 0;
  mChar2 = '_';
  m16Bit = 0;
}

void SerialDevice::writeMessage(char h1, int32_t v1, char h2, int16_t v2) {
  mPort.write(h1); // Write first character to serial port
  mPort.write(v1 & 0xff); // Write 32 bit integer to serial port from least to most significant bytes
  mPort.write((v1 >> 8) & 0xff);
  mPort.write((v1 >> 16) & 0xff);
  mPort.write((v1 >> 24) & 0xff);
  mPort.write(h2); // Write second character to serial port
  mPort.write(v2 & 0xff); // Write 16 bit integer to serial port from least to most significant byte
  mPort.write((v2 >> 8) & 0xff);
}

// Read 8 byte messages from the provided serial port in specific data order.
// Returns the first character of the data packet and stores remaining values in global variables
char SerialDevice::readMessage() { 
  mChar1 = mPort.read();
  m32Bit = mPort.read() | mPort.read() << 8 | mPort.read() << 16 | mPort.read() << 24; // Read 32 bit integer from serial port 
  mChar2 = mPort.read(); // Read second character from serial port
  m16Bit = mPort.read() | mPort.read() << 8; // Read 16 bit integer from serial port 
  return mChar1;
}

bool SerialDevice::awaitMessage(char msg, bool toggle, int pinToToggle) {
  char header;
  int32_t lightToggleTime = 0;

  while (true){
    if (messageAvailable()) {
      header = readMessage();
      if (header == msg) {
        return true;
      }
    } else if (millis() - lightToggleTime > 500 && toggle) {
      digitalWrite(pinToToggle, !digitalRead(pinToToggle));
      lightToggleTime = millis();
    }
  }
  return false;
}


bool SerialDevice::messageAvailable() {
  if (mPort.available() > 7) {
    return true;
  } else {
    return false;
  }
}


void SerialDevice::flushReceiveBuffer() {
  while(mPort.available() > 0) {
    mPort.read();
  }
}


int32_t SerialDevice::get32BitValue() { return m32Bit; }

char SerialDevice::getChar2() { return mChar2; }

int16_t SerialDevice::get16BitValue() { return m16Bit; }