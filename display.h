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

  //set current temperature and time
  void set_current_temp(float T);
  void setRemainingTime(uint32_t time_in_msec);

  //get the set point values
  float    getTempSetpoint();
  uint32_t getTimeSetpoint();

 private:
  uint8_t icursor=0;
  const uint8_t xcursor[8]={7,17,29,38,76,85,99,108};
  uint8_t setpoint_val[8]={2,5,0,0,0,5,0,0};

  float current_temp;
  uint32_t remaining_time_msec=0;

  uint8_t cursorColor=1;

  U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled;

  void disp_remaining_time();
  void disp_current_temp();
  void disp_cursor();
  void disp_setpoints();
  void disp_glyph();

};

#endif
