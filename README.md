# Brewing temperature control v2.0

## Upgrade from v1.0 :
- added a rotary encoder to control the electric power supply from 0% to
  100%. this is done using pwm and the SSR relay

This device is a temperature controler that is meant to maintain water temperature for a given amount of time.
The user can input the setpoint temperature and the duration the temperature should be maintained.

## HMI

Two butons serve for input. The right button switches the cursor to the right next figure. The left button increments
the displayed value. Pressing and holding both buttons start the heating process.

## Hardware
--
- OLED display
- SSR relay to swith the power to the resistive heater
- atmega328
- 2 buttons
- a rotary encoder switch

## ToDo
- Documentation



Notes :
-------------
if "arduino:avr:328bb" is not found, modify the */Users/xxxxxxx/Library/Arduino15/packages/arduino/hardware/avr/1.x.y/* folder