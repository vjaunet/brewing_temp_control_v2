/*
  oled display wrapper librairie

  Modified:     01/01/2018

  Author:       Vincent Jaunet <vincent.jaunet(at)hotmail(dot)com>
 */

#ifndef display_h
#define display_h

#include "Arduino.h"
#include <U8g2lib.h>
#include <Wire.h>

#define NBR_CHARACTER 7

class display
{
 public:
  display();
  void start();
  void refresh_display();

  //cursor related functions
  void hide_cursor();
  void show_cursor();

  //functions for the setpoints
  void change_cursor_pos();
  void increment_cursor_value();

  //set current temperature and power
  void set_current_temp(float T);
  void set_current_power(float P);

  //get the set point values
  float  getTempSetpoint();
  float  getPowerSetpoint();

 private:
  uint8_t icursor=0;
  const uint8_t xcursor[NBR_CHARACTER]={7,17,29,38,76,85,94};
  uint8_t  setpoint_val[NBR_CHARACTER]={2,5,0,0,1,0,0};

  float current_temp;
  uint8_t current_power;

  uint8_t cursorColor=1;

  U8G2_SSD1306_128X64_NONAME_1_HW_I2C oled;

  void update_power_char();
  void disp_current_temp();
  void disp_current_power();
  void disp_cursor();
  void disp_setpoints();
  void disp_glyph();

};

#endif
