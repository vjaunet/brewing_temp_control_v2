# Brewing temperature control

This device is a temperature controler that is meant to maintain water temperature for a given amount of time.
The user can input the setpoint temperature and the duration the temperature should be maintained.

HMI
--
Two butons serve for input. The right button switches the cursor to the right next figure. The left button increments 
the displayed value. Pressing and holding both buttons start the heating process.

Hardware
--
- OLED display
- SSR relay to swith the power to the resistive heater
- atmega328
- 2 buttons

ToDo
--
- PI controller for the temperature
- Documentation
