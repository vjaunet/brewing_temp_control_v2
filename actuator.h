/*
 Actuator control library

  This library is meant to used for 2 states actuators

  Modified:     24 November 2014

  Author:       Vincent Jaunet <v.jaunet(at)gmail(dot)com>
 */


#ifndef actuator_h
#define actuator_h

#include "Arduino.h"

class actuator
{
 public:
  actuator(uint8_t pin);
  void open();
  void close();
  void on();
  void off();
  void toggle();
  uint8_t is_open();
  uint8_t is_on();

 private:
  uint8_t m_pin;
  uint8_t m_status;
};

#endif
