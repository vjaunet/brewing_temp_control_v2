/*
 Actuator control library

  This library is meant to used for 2 states actuators

  Modified:     24 November 2014

  Author:       Vincent Jaunet <v.jaunet(at)gmail(dot)com>
 */

#include "Arduino.h"
#include "actuator.h"

actuator::actuator(uint8_t pin)
{
  pinMode(pin, OUTPUT);
  m_pin = pin;
  digitalWrite(m_pin,LOW);
  m_status = 0;
}

void actuator::on()
{
  open();
}
void actuator::off()
{
  close();
}

void actuator::open()
{
  if (m_status!=1)
    {
      digitalWrite(m_pin,HIGH);
      m_status = 1;
    }
}

void actuator::close()
{
  if (m_status!=0)
    {
      digitalWrite(m_pin,LOW);
      m_status = 0;
    }
}

void actuator::toggle()
{
  digitalWrite(m_pin,!m_status);
  m_status = !m_status;
}

uint8_t actuator::is_open()
{
  return m_status;
}

uint8_t actuator::is_on()
{
  return m_status;
}
