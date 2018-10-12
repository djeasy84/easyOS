#!/bin/bash

export PATH=$PATH:../../../AVR-GCC/bin/

make clean

make atmega328

cp optiboot_atmega328.hex ../../../../Bootloader/arduinoNANO/optiboot_16MHz/

make clean
