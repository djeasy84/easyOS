#!/bin/bash

export PATH=$PATH:../../../AVR-GCC/bin/

make clean

make atmega328_8mhz

cp optiboot_atmega328_8mhz.hex ../../../../Bootloader/arduinoNANO/optiboot_8MHz/

make clean
