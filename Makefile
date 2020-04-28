# --- atmega328p on breadboard
#BOARD_TAG      = 328bb
#F_CPU          = 8000000L
#MCU            = atmega328p


all:
	arduino-cli compile \
	--fqbn=arduino:avr:328bb \
	--warnings all \
	../brewing_temp_control_v2

upload:
