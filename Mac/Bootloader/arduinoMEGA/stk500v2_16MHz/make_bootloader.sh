#!/bin/bash

export PATH=$PATH:../../../AVR-GCC/bin/

make clean

make mega2560

cp stk500boot_v2_mega2560.hex ../../../../Bootloader/arduinoMEGA/stk500v2_16MHz/

make clean
