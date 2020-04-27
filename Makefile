
# --- Chinese Nano V3.0 --------------
# BOARD_TAG      = nano
# F_CPU          = 16000000L
# MCU            = atmega328p

# PORT=/dev/cu.wchusbserial1420
# AVRDUDE_ARD_BAUDRATE = 57600
# ISP_PROG     = avrisp
## AVRDUDE_OPTS = -v

# --- atmega328p on breadboard
BOARD_TAG      = 328bb
F_CPU          = 8000000L
MCU            = atmega328p

#ARDUINO_PORT=/dev/cu.wchusbserial1420
ISP_PROG=usbasp
AVRDUDE_ARD_BAUDRATE = 57600
AVRDUDE_OPTS = -v


#====================================================
ARDUINO_LIBS = SPI Wire U8g2 OneWire

MONITOR_BAUDRATE=9600

#----------------------------------------------
#include the Arduino Makefile
include /usr/local/opt/arduino-mk/Arduino.mk
