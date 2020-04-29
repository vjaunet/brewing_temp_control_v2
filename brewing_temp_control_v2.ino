/*===========================================================

  Device to monitor the temperature of a brewing pot

  author: vincent jaunet
  date  : dec. 2017

  licence : GPL v3.0

============================================================== */
#include "Arduino.h"
#include <OneWire.h>
#include "DallasTemperature.h"
#include "display.h"
#include "timer1.h"

//seems like the 328bb does not define the board correctly
// #define __AVR_ATmega328P__ was added to the header
#include "Encoder.h"

#define DEBUG

//Some global variables
float cur_temp;
uint8_t cur_power;

/* ============================================================= */
// Data wire is plugged into pin 4 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

/* ============================================================= */
// OLED display
display display;

//* ============================================================ */
// Encoder
Encoder encoder(3,4);
long encoderPos  = 0;


/*============================================================== */
// defining the heater

//we have a 50Hz power input so the duration of the
//high state should be a multiple of a 20 ms period
#define PWM_FREQ 5 // 0.1*50Hz
void heater_on(float power=1.0){
  connect_timer1_output();
  set_timer1_DC(power);
}

void heater_off(){
  //unset the timer outputs
  //should be low state
  disconnect_timer1_output();
}


/* ============================================================= */
// Menu buttons
#define BTN_R PB0
#define BTN_L PB2
void buttons_init(){
 // Buttons as input, LED as output they are all in PORTB
 // Set the internal pull-up resistor
 DDRB  &= ~((1 << BTN_R) | (1 << BTN_L));
 PORTB |=   (1 << BTN_R) | (1 << BTN_L);

 PCICR  |= (1 << PCIE0);   // set PCIE0 to enable PCMSK0 scan
 PCMSK0 |= (1 << PCINT0) | (1 << PCINT2);  // set PCINTx to trigger an interrupt on state change
 sei(); //turn interrupt on
}

#define BTN_R_pressed (0x01)
#define BTN_L_pressed (0x02)
#define BTN_RandL_pressed (0x03)
uint8_t button_flag=0x00;
ISR (PCINT0_vect) //PCINT0_vect checks PCINT0-7 pins
{

  if ( (PINB & (1 << BTN_R)) == 0) {
    //PB1 went down
    button_flag |= BTN_R_pressed;
  }

  if ( (PINB & (1 << BTN_L)) == 0) {
    //PB2 went down
    button_flag |= BTN_L_pressed;
  }

}

/* useful wait without delay */
void wait_ms_(uint32_t delay){
  uint32_t start_time=millis();
  while(millis()-start_time < delay);
  return;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
// the setup routine runs once
void setup() {

#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Started");
#endif

  // Start up the library - default resolution 12bits
  // Note that if the wait for conversion flag is enabled (default)
  // hence the requestTemperatures() do not return before the
  // conversion is finished.
  sensors.begin();
  sensors.setResolution(10);
  sensors.requestTemperatures();
  display.set_current_temp(sensors.getTempCByIndex(0));

  // sensors.setWaitForConversion(false);
  // sensors.setCheckForConversion(false);

  // prepare timer 1 to PWM the heater
  // and initialize the timer1 pin 9 digital pins as outputs.
  DDRB |= (1<<PB1);
  set_timer1_mode();
  set_timer1_freqDC(PWM_FREQ, 1.0);
  disconnect_timer1_output();
  //GTCCR = 0; //Go all timers //== NOT NEEDED ???==//

  // Start the OLED display
  display.start();

  //initialize menu buttons
  buttons_init();

}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
// the loop routine runs over and over
bool start_process=false;

uint8_t Nloop=0;

float last_temp=99.0;
float meas_temp;
void loop() {

  /*---------------------------------------------------------------*/
  // check the button flags and do the corresponding actions
  if (button_flag == BTN_R_pressed &&
      button_flag != BTN_L_pressed &&
      !start_process)
    {
      button_flag &= ~BTN_R_pressed;

      /*===== debouncing =====*/
      wait_ms_(3);
      if ((PINB & (1 << BTN_R)) == 0)
	display.change_cursor_pos();
    }
  else if (button_flag == BTN_L_pressed &&
	   button_flag != BTN_R_pressed &&
	   !start_process)
    {
      button_flag &= ~BTN_L_pressed;

      /*===== debouncing =====*/
      wait_ms_(3);
      if ((PINB & (1 << BTN_L)) == 0)
	display.increment_cursor_value();

    }
  else if (button_flag == BTN_RandL_pressed)
    {
      button_flag &= ~BTN_RandL_pressed;

      //check for long press of both buttons
      wait_ms_(300);
      if ((PINB & (1 << BTN_L)) == 0 ||
	  (PINB & (1 << BTN_R)) == 0) {
	//if still pressed : go !
	start_process = !start_process;
      }
    }

  /*---------------------------------------------------------------*/
  // Check the encoder state and modulate the power accordingly
  long newEncoderPos = encoder.read();
  if (newEncoderPos != encoderPos){
    long delta =(newEncoderPos-encoderPos);
    float cur_pow = display.getPowerSetpoint();
#ifdef DEBUG
    Serial.println( delta );
    Serial.println( (uint8_t)( cur_pow *100) );
#endif
    display.set_current_power( cur_pow + delta*0.01);
    encoderPos = newEncoderPos;
#ifdef DEBUG
    cur_pow = display.getPowerSetpoint();
    Serial.println( cur_pow );
#endif

  }

  /*---------------------------------------------------------------*/
  // for display purposes :
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus.
  if (!start_process && Nloop++ > 10) {
    Nloop=0;
    sensors.requestTemperatures();
    meas_temp=sensors.getTempCByIndex(0);
    display.set_current_temp(meas_temp);
    last_temp = meas_temp;
  }

  /*---------------------------------------------------------------*/
  // temperature control and timing operation
  if (start_process){

    //turn off the cursor again and reset its position
    display.hide_cursor();


    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus.
    // wait for the conversion to have finished and
    // update the displayed temp value
    sensors.requestTemperatures();
    meas_temp=sensors.getTempCByIndex(0);
    if (meas_temp == -127) {
      //a measurement error occured
#ifdef DEBUG
      Serial.println(" error in temp measurement");
#endif
      heater_off();
      wait_ms_(200);
      return;
    }

    //TODO : start heating depending on whether temperature is
    //TODO:  increasing or decreasing

    // decide to heat or not.
    if ( meas_temp < (display.getTempSetpoint() - 2.0)) {
      //need to heat
      heater_on(display.getPowerSetpoint());

    } else if ( meas_temp < display.getTempSetpoint() &&
		meas_temp >= display.getTempSetpoint() - 2.0 ) {
      //the output power is set according to the temperature difference
      //the closer we are to the setpoint the less power we input to
      //avoid overheating
      float pow = display.getPowerSetpoint()*
	abs(meas_temp-display.getTempSetpoint())/2.0;
      heater_on(pow);

    } else {
      //not need to heat anymore
      heater_off();
    }


    /*---------------------------------------------------------------*/
    //reccord the last measured temperature
    last_temp = meas_temp;

    //update the displayed temperature
    display.set_current_temp(meas_temp);

  } else {
    // user may have pressed both buttons another time
    // to cancel the process
    heater_off();

    //display back the cursor
    display.show_cursor();
  }

  /*---------------------------------------------------------------*/
  // refresh the display
  display.refresh_display();

}
