/*
  oled display wrapper librairie

  Modified:     01/01/2018

  Author:       Vincent Jaunet <vincent.jaunet(at)hotmail(dot)com>
 */

#include "Arduino.h"
#include "display.h"

display::display() : oled(U8G2_R0)
{
  // Nothing to be done here
}

void display::start(){
  oled.begin();
  oled.setFontMode(1);
}

void display::set_current_temp(float T){
  current_temp = T;
}

void display::setRemainingTime(uint32_t time_in_msec){
  remaining_time_msec = time_in_msec;
}

float display::getTempSetpoint(){

  return setpoint_val[0]*10. +
    setpoint_val[1] +
    setpoint_val[2]*.1 +
    setpoint_val[3]*.01;
}

uint32_t display::getTimeSetpoint(){

  return (uint32_t)
    60000*(setpoint_val[4]*10 +
	   setpoint_val[5]) +
    1000*(setpoint_val[6]*10 +
	  setpoint_val[7]);
}


void display::change_cursor_pos(){
  //increment the position of the cursor
  icursor++;
  if (icursor > 7) icursor=0;
}

void display::increment_cursor_value(){
  //increment the value of the number at the cursor position
  setpoint_val[icursor] = setpoint_val[icursor]+1;
  if (setpoint_val[icursor] > 9) setpoint_val[icursor]=0;
}

void display::disp_current_temp(){
  // display measured temperature
  oled.setDrawColor(1);
  oled.setFont(u8g2_font_helvR12_tr);

  //temperature
  oled.setCursor(9, 54);
  oled.print(current_temp);
}

void display::disp_remaining_time(){
  // display remaining time
  oled.setDrawColor(1);
  oled.setFont(u8g2_font_helvR12_tr);

  // timing
  oled.setCursor(78, 54);
  uint8_t min10=remaining_time_msec/600000;
  oled.print(min10,DEC);
  uint8_t min=(remaining_time_msec - 600000*min10)/60000;
  oled.print(min,DEC);
  oled.print(":");
  uint8_t sec=(remaining_time_msec - (10*min10 + min)*60000)/1000;
  oled.print(sec/10,DEC);
  oled.print(sec-10*(sec/10),DEC);
}

void display::disp_cursor(){
  //draw a box at the cursor position
  oled.setDrawColor(cursorColor);
  oled.drawBox(xcursor[icursor], 21, 9, 14);
}

void display::hide_cursor(){
  //set transparent cursor
  cursorColor = 0;
  //reset its position
  icursor = 0;
}

void display::show_cursor(){
  cursorColor = 1;
}

void display::disp_setpoints(){
  // display the bold setpoints
  oled.setDrawColor(2); //for cursor overlay
  oled.setFont(u8g2_font_helvB12_tr);

  for(uint8_t i=0; i < 8; i++){
    oled.setCursor(xcursor[i], 34);
    oled.print(setpoint_val[i]);
  }

  // display punctuation
  oled.setCursor(26, 35);
  oled.print(".");

  oled.setCursor(93, 34);
  oled.print(":");

}

void display::disp_glyph(){
  //display glyphs on the top of the display
  oled.setFont(u8g2_font_unifont_t_symbols);
  oled.drawGlyph(26,14,0x2103); // °C
  oled.drawGlyph(90,14,0x23f1); // Chrono
  oled.drawLine(64,0,64,63);
}

void display::refresh_display(){
  //create and send the buffer to the oled
  //screen.
  oled.clearBuffer();
  disp_glyph();
  disp_cursor();
  disp_setpoints();
  disp_current_temp();
  disp_remaining_time();
  oled.sendBuffer();
}
