#!/bin/bash
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

rsync -azv brewing_temp_control_v2.arduino.avr.328bb.hex ElecRpi:~/BURN/.
ssh ElecRpi './BURN/burn2atmega328.sh BURN/brewing_temp_control_v2.arduino.avr.328bb.hex'
