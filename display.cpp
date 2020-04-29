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

void display::set_current_power(float P){
  // test the values of intput Power
  if ( P > 1.0) {
    P = 1.0;
  } else if (P < 0.0){
    P = 0;
  }

  //update the values to be diaplayed
  current_power = (uint8_t)( P * 100.0);
  update_power_char();
}

void display::update_power_char(){
  setpoint_val[4]=(current_power/100)%10; //hundreds
  setpoint_val[5]=(current_power/10)%10;  //tens
  setpoint_val[6]=current_power%10;       //ones
}


float display::getTempSetpoint(){
  return setpoint_val[0]*10. +
    setpoint_val[1] +
    setpoint_val[2]*.1 +
    setpoint_val[3]*.01;
}

float display::getPowerSetpoint(){
  return ((float)(setpoint_val[4]*100 +
		  setpoint_val[5]*10 +
		  setpoint_val[6]))/100.;
}

void display::change_cursor_pos(){
  //increment the position of the cursor
  icursor++;
  if (icursor > NBR_CHARACTER-1) icursor=0;
}

void display::increment_cursor_value(){

  //increment the value of the number at the cursor position
  setpoint_val[icursor] = setpoint_val[icursor]+1;
  if (icursor == NBR_CHARACTER-3){
    if (setpoint_val[icursor] > 1) setpoint_val[icursor]=' ';
  } else {
    if (setpoint_val[icursor] > 9) setpoint_val[icursor]=0;
  }

  //if power is > than 100%, set it to 100%
  if (setpoint_val[NBR_CHARACTER-3] == 1 &&
      (setpoint_val[NBR_CHARACTER-2] != 0 ||
       setpoint_val[NBR_CHARACTER-1] != 0)) {
    setpoint_val[NBR_CHARACTER-3] = 0;
  }
}

void display::disp_current_temp(){
  // display measured temperature
  oled.setDrawColor(1);
  oled.setFont(u8g2_font_helvR12_tr);

  //temperature
  oled.setCursor(9, 54);
  oled.print(current_temp);
}

// void display::disp_current_power(){
//   // display actual used powwer
//   oled.setDrawColor(1);
//   oled.setFont(u8g2_font_helvR12_tr);

//   //temperature
//   oled.setCursor(79, 54);
//   oled.print((uint8_t) current_power*100);
// }

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

  for(uint8_t i=0; i < NBR_CHARACTER; i++){
    oled.setCursor(xcursor[i], 34);
    oled.print(setpoint_val[i]);
  }

  // display punctuation
  oled.setCursor(26, 35);
  oled.print(".");

  oled.setCursor(103, 34);
  oled.print("%");

}

void display::disp_glyph(){
  //display glyphs on the top of the display
  oled.setFont(u8g2_font_unifont_t_symbols);
  oled.drawGlyph(26,14,0x2103); // °C
  oled.drawGlyph(90,14,0x2615); // Heating Cup
  oled.drawLine(64,0,64,63);
}

void display::refresh_display(){
  //create and send the buffer to the oled
  //screen.

  //oled.clearBuffer();

  oled.firstPage();
  do {
    disp_glyph();
    disp_cursor();
    disp_setpoints();
    disp_current_temp();
    //disp_current_power();
  } while( oled.nextPage() );

  //oled.sendBuffer();
}
