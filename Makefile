# --- atmega328p on breadboard
#BOARD_TAG      = 328bb
#F_CPU          = 8000000L
#MCU            = atmega328p


all:
	@arduino-cli compile \
		--fqbn=arduino:avr:328bb \
		--warnings all \
		../brewing_temp_control_v2

upload:
	@rsync -azv brewing_temp_control_v2.arduino.avr.328bb.hex ElecRpi:~/BURN/.
	@ssh ElecRpi './BURN/burn2atmega328.sh BURN/brewing_temp_control_v2.arduino.avr.328bb.hex'
